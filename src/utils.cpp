#include "utils.h"

float minkowski_distance(Point *a, Point *b, int n, bool update_stats)
{
    float result = 0;

    for (int i = 0; i < a->coords.size(); i++)
        result += pow(a->coords[i] - b->coords[i], n);

    if (update_stats)
    {
        a->metric_calculate_count++;
        b->metric_calculate_count++;
    }

    return pow(result, 1.0 / n);
}

float get_from_distance_point_matrix(vector<vector<float>> *distance_points_matrix, int a, int b)
{
    int min_value = min(a, b);
    int max_value = max(a, b);
    return distance_points_matrix->at(max_value)[min_value];
}