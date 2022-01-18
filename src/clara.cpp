#include "clara.h"

vector<int> clara(vector<vector<float>> points, int cluster_count, int minkowski_n, int max_swap)
{
    // vector<int> res;

    return pam(points, cluster_count, minkowski_n, max_swap);
}

vector<int> pam(vector<vector<float>> points, int cluster_count, int minkowski_n, int max_swap)
{
    vector<vector<float>> distance_points_matrix(points.size());
    for (int i = 0; i < points.size(); ++i)
    {
        vector<float> distances_tmp(points.size());
        for (int j = 0; j < points.size(); ++j)
        {
            distances_tmp[j] = minkowski_distance(points[i], points[j], minkowski_n);
        }
        distance_points_matrix[i] = distances_tmp;
    }

    vector<vector<int>> centroids_data = init_centroids(points, cluster_count);
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
                    tuple<float, float> res = calculate_d_j(distance_points_matrix, centroids, j_value);
                    float Dj = get<0>(res);
                    float Ej = get<1>(res);

                    if (distance_points_matrix[j_value][i_value] > Dj)
                        K = min(distance_points_matrix[j_value][h_value] - Dj, (float)0.0);
                    if (distance_points_matrix[j_value][i_value] == Dj)
                        K = min(distance_points_matrix[j_value][h_value], Ej) - Dj;

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

    return centroids;
}

tuple<float, float> calculate_d_j(vector<vector<float>> distance_points_matrix, vector<int> centroids, int j)
{
    float min_d_j = numeric_limits<float>::max();
    float second_min_d_j = numeric_limits<float>::max();

    for (int centroid : centroids)
    {
        if (distance_points_matrix[centroid][j] < min_d_j)
        {
            second_min_d_j = min_d_j;
            min_d_j = distance_points_matrix[centroid][j];
        }
    }
    return {min_d_j, second_min_d_j};
}

//BUILD BEZ OPTYMALIZACJI - i fajnie! mozna tutaj dac optymalizacje jako druga wersje - wybrac obiekty o najmniejszej sumie odleglosci do wszystkich innych
vector<vector<int>> init_centroids(vector<vector<float>> points, int cluster_count)
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