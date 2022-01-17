#include "dbscan.h"

vector<int> eps_neighborhood(
    vector<float> ref,
    vector<vector<float>> points,
    float eps,
    int minkowski_n)
{
    vector<int> result;

    for (int i = 0; i < points.size(); i++)
        if (minkowski_distance(ref, points[i], minkowski_n) < eps)
            result.push_back(i);

    return result;
}

vector<int> dbscan(vector<vector<float>> points, float eps, int min_pts, int minkowski_n)
{
    const int NON_VISITED_YET = -2;
    const int NON_CORE = -1;

    vector<int> clustering;
    for (int i = 0; i < points.size(); i++)
        clustering.push_back(NON_VISITED_YET);

    int current_cluster = -1;
    bool durning_create_cluster = false;

    for (int current_point = 0; current_point < points.size(); current_point++)
    {
        if (clustering[current_point] >= NON_CORE)
            continue;

        queue<int> seeds;

        seeds.push(current_point);

        while (!seeds.empty())
        {
            vector<int> current_eps_neighborhood = eps_neighborhood(
                points[seeds.front()], points, eps, minkowski_n);

            if (current_eps_neighborhood.size() >= min_pts)
            {
                if (!durning_create_cluster)
                {
                    durning_create_cluster = true;
                    current_cluster++;
                }

                clustering[current_point] = current_cluster;

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

    return clustering;
}