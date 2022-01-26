#include "clara.h"

vector<int> clara(
    vector<Point *> points,
    int cluster_count,
    int random_group_count,
    int random_group_size,
    int minkowski_n,
    int max_swap,
    bool multithread,
    bool optimized_pam_init)
{
    cluster_count = min(cluster_count, (int)points.size());

    random_group_size = min(random_group_size, (int)points.size());
    vector<vector<Point *>> datasets(random_group_count);
    for (int i = 0; i < random_group_count; ++i)
        datasets[i] = draw_data(points, random_group_size);

    float best_dist = numeric_limits<float>::max();
    vector<int> best_clustering;

    if (multithread)
    {
        //optimization!
    }
    else
    {
        for (int i = 0; i < random_group_count; ++i)
        {
            std::cout << "PAM number: " << i << endl;
            vector<Point *> centroids = pam(datasets[i], cluster_count, minkowski_n, max_swap, optimized_pam_init);
            auto res = prepare_clustering_based_on_centroids(centroids, points, minkowski_n);
            float curr_dist = get<0>(res);
            if (curr_dist < best_dist)
            {
                best_clustering = get<1>(res);
                best_dist = curr_dist;
            }
        }
    }
    return best_clustering;
}

tuple<float, vector<int>> prepare_clustering_based_on_centroids(vector<Point *> centroids, vector<Point *> points, int minkowski_n)
{
    vector<int> clustering(points.size());
    float distance = 0;

    for (int i = 0; i < points.size(); ++i)
    {
        float best_centroid_dist = numeric_limits<float>::max();
        int best_centroid;
        for (int j = 0; j < centroids.size(); ++j)
        {
            float dist = minkowski_distance(points[i], centroids[j], minkowski_n);
            if (dist < best_centroid_dist)
            {
                best_centroid_dist = dist;
                best_centroid = j;
            }
        }
        clustering[i] = best_centroid;
    }

    return {distance, clustering};
}

vector<Point *> draw_data(vector<Point *> points, int random_group_size)
{
    random_device rd;
    mt19937 g(rd());

    vector<int> candidates(points.size());
    iota(candidates.begin(), candidates.end(), 0);
    shuffle(candidates.begin(), candidates.end(), g);

    vector<int> selected_data_indexes(candidates.begin(), candidates.begin() + random_group_size);

    vector<Point *> results(random_group_size);

    for (int i = 0; i < random_group_size; ++i)
        results[i] = points[selected_data_indexes[i]];

    return results;
}

float get_from_distance_point_matrix(vector<vector<float>> *distance_points_matrix, int a, int b)
{
    int min_value = min(a, b);
    int max_value = max(a, b);
    return distance_points_matrix->at(max_value)[min_value];
}

vector<Point *> pam(vector<Point *> points, int cluster_count, int minkowski_n, int max_swap, bool optimized_pam_init)
{
    vector<vector<float>> distance_points_matrix(points.size());
    for (int i = 0; i < points.size(); ++i)
    {
        vector<float> distances_tmp(i + 1);
        for (int j = 0; j < i; ++j)
        {
            distances_tmp[j] = minkowski_distance(points[i], points[j], minkowski_n);
        }
        distances_tmp[i] = 0;
        distance_points_matrix[i] = distances_tmp;
    }

    vector<vector<int>> centroids_data = init_centroids(points, cluster_count, optimized_pam_init);
    vector<int> centroids = centroids_data[0];
    vector<int> non_centroids = centroids_data[1];

    for (int swap_index = 0; swap_index < max_swap; ++swap_index)
    {
        int min_T_i;
        int min_T_h;
        float min_T = numeric_limits<float>::max();

        for (int i = 0; i < centroids.size(); ++i)
        {
            for (int h = 0; h < non_centroids.size(); ++h)
            {
                float T = 0;
                for (int j = 0; j < non_centroids.size(); j++)
                {
                    if (j == h)
                        continue;

                    int i_value = centroids[i];
                    int h_value = non_centroids[h];
                    int j_value = non_centroids[j];

                    float K = 0;
                    tuple<float, float> res = calculate_d_j(&distance_points_matrix, centroids, j_value);
                    float Dj = get<0>(res);
                    float Ej = get<1>(res);

                    if (get_from_distance_point_matrix(&distance_points_matrix, j_value, i_value) > Dj)
                        K = min(get_from_distance_point_matrix(&distance_points_matrix, j_value, h_value) - Dj, (float)0.0);
                    if (get_from_distance_point_matrix(&distance_points_matrix, j_value, i_value) == Dj)
                        K = min(get_from_distance_point_matrix(&distance_points_matrix, j_value, h_value), Ej) - Dj;

                    T += K;
                }
                if (T < min_T)
                {
                    min_T = T;
                    min_T_i = i;
                    min_T_h = h;
                }
            }
        }
        if (min_T > 0)
            break;

        int tmp = centroids[min_T_i];
        centroids[min_T_i] = non_centroids[min_T_h];
        non_centroids[min_T_h] = tmp;
    }

    vector<Point *> res(cluster_count);
    for (int i = 0; i < cluster_count; ++i)
        res[i] = points[centroids[i]];

    return res;
}

tuple<float, float> calculate_d_j(vector<vector<float>> *distance_points_matrix, vector<int> centroids, int j)
{
    float min_d_j = numeric_limits<float>::max();
    float second_min_d_j = numeric_limits<float>::max();

    for (int centroid : centroids)
    {
        if (get_from_distance_point_matrix(distance_points_matrix, centroid, j) < min_d_j)
        {
            second_min_d_j = min_d_j;
            min_d_j = get_from_distance_point_matrix(distance_points_matrix, centroid, j);
        }
    }
    return {min_d_j, second_min_d_j};
}

//BUILD BEZ OPTYMALIZACJI - i fajnie! mozna tutaj dac optymalizacje jako druga wersje - wybrac obiekty o najmniejszej sumie odleglosci do wszystkich innych
vector<vector<int>> init_centroids(vector<Point *> points, int cluster_count, bool optimized_pam_init)
{
    //init random numer generator
    random_device rd;
    mt19937 g(rd());

    //draw initial centroids
    vector<int> centroid_candidates(points.size());
    iota(centroid_candidates.begin(), centroid_candidates.end(), 0);
    shuffle(centroid_candidates.begin(), centroid_candidates.end(), g);

    vector<int> centroids(centroid_candidates.begin(), centroid_candidates.begin() + cluster_count);
    vector<int> non_centroids(centroid_candidates.begin() + cluster_count, centroid_candidates.end());
    vector<vector<int>> res(2);
    res[0] = centroids;
    res[1] = non_centroids;
    return res;
}