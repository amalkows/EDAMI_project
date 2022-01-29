#include "dbscan.h"

vector<int> eps_neighborhood(
    int ref,
    float eps,
    vector<vector<float>> *distance_points_matrix)
{
    vector<int> result;

    for (int i = 0; i < distance_points_matrix->size(); i++)
        if (get_from_distance_point_matrix(distance_points_matrix, ref, i) < eps)
            result.push_back(i);

    return result;
}

tuple<vector<int>, vector<int>> dbscan(vector<Point *> points, float eps, int min_pts, int minkowski_n)
{
    //time of prepare matrix
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

    const int NON_VISITED_YET = -2;
    const int NON_CORE = -1;

    vector<int> clustering, points_types(points.size());

    for (int i = 0; i < points.size(); i++)
        clustering.push_back(NON_VISITED_YET);

    int current_cluster = -1;
    bool durning_create_cluster = false;

    //time of processing
    for (int current_point = 0; current_point < points.size(); current_point++)
    {
        if (clustering[current_point] >= NON_CORE)
            continue;

        queue<int> seeds;

        seeds.push(current_point);

        // debug current point oddzielacz

        while (!seeds.empty())
        {

            vector<int> current_eps_neighborhood = eps_neighborhood(
                seeds.front(), eps, &distance_points_matrix);

            //debug seed - eps neighborhood

            if (current_eps_neighborhood.size() >= min_pts)
            {
                if (!durning_create_cluster)
                {
                    durning_create_cluster = true;
                    current_cluster++;
                }

                clustering[current_point] = current_cluster;
                points_types[seeds.front()] = 1;

                for (int neighbor_index = 0; neighbor_index < current_eps_neighborhood.size(); neighbor_index++)
                {
                    if (clustering[current_eps_neighborhood[neighbor_index]] == NON_VISITED_YET)
                    {
                        clustering[current_eps_neighborhood[neighbor_index]] = current_cluster;
                        seeds.push(current_eps_neighborhood[neighbor_index]);
                    }
                    else if (clustering[current_eps_neighborhood[neighbor_index]] == NON_CORE)
                        clustering[current_eps_neighborhood[neighbor_index]] = current_cluster;
                }
            }
            else if (clustering[seeds.front()] == NON_VISITED_YET)
                clustering[seeds.front()] = NON_CORE;

            seeds.pop();
        }
        durning_create_cluster = false;
    }

    return std::make_tuple(clustering, points_types);
}