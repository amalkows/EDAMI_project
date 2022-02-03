#include "metrics.h"

float purity_score(vector<int> true_clusters, vector<int> predicted_clusters)
{
    int n = true_clusters.size();
    int result = 0;
    int true_clusters_count = *max_element(true_clusters.begin(), true_clusters.end()) + 1;
    int predicted_clusters_count = *max_element(predicted_clusters.begin(), predicted_clusters.end()) + 1;

    for (int pred_cluster_index = 0; pred_cluster_index < predicted_clusters_count; pred_cluster_index++)
    {
        int best_intersection = 0;
        for (int true_cluster_index = 0; true_cluster_index < true_clusters_count; true_cluster_index++)
        {
            int intersection = 0;
            for (int i = 0; i < n; i++)
                if (predicted_clusters[i] == pred_cluster_index && true_clusters[i] == true_cluster_index)
                    intersection++;

            if (intersection > best_intersection)
                best_intersection = intersection;
        }
        result += best_intersection;
    }
    return (float)result / n;
}

int factorial(int i)
{
    int result = 1;

    for (int j = i; j > 0; j--)
        result = result * j;

    return result;
}

std::tuple<float, int, int, int> rand_score(vector<int> true_clusters, vector<int> predicted_clusters)
{
    int n = true_clusters.size();

    int TP = 0;
    int TN = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if ((true_clusters[i] == true_clusters[j]) && (predicted_clusters[i] == predicted_clusters[j]))
                TP++;
            if ((true_clusters[i] != true_clusters[j]) && (predicted_clusters[i] != predicted_clusters[j]))
                TN++;
        }
    }

    int numerator = TP + TN;
    int denominator = (n * (n - 1)) / 2;

    return std::make_tuple((float)numerator / (float)denominator, TP, TN, denominator);
}

vector<Point *> calculate_centroids(vector<Point *> points, vector<int> clusters)
{
    int cluster_count = *max_element(clusters.begin(), clusters.end()) + 1;
    int dim = points[0]->coords.size();

    vector<Point *> result(cluster_count);
    for (int i = 0; i < cluster_count; ++i)
        result[i] = new Point(dim);

    vector<int> cluster_size(cluster_count);

    for (int k = 0; k < points.size(); k++)
        if (clusters[k] >= 0)
        {
            for (int i = 0; i < dim; i++)
                result[clusters[k]]->coords[i] += points[k]->coords[i];
            cluster_size[clusters[k]]++;
        }

    for (int j = 0; j < cluster_count; j++)
        for (int i = 0; i < dim; i++)
            result[j]->coords[i] = result[j]->coords[i] / cluster_size[j];

    return result;
}

vector<float> calculate_average_distance_of_points_to_centroids(
    vector<Point *> points,
    vector<int> clusters,
    vector<Point *> centroids)
{
    int cluster_count = centroids.size();
    int dim = points[0]->coords.size();
    vector<int> cluster_size(cluster_count);

    vector<float> result(cluster_count);

    for (int k = 0; k < points.size(); k++)
        if (clusters[k] >= 0)
        {
            result[clusters[k]] += minkowski_distance(points[k], centroids[clusters[k]], 2, false);
            cluster_size[clusters[k]]++;
        }

    for (int i = 0; i < cluster_count; i++)
        result[i] = result[i] / cluster_size[i];

    return result;
}

vector<vector<float>> calculate_distance_between_centroids(vector<Point *> centroids)
{
    int n = centroids.size();
    vector<vector<float>> result(n, vector<float>(n));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i != j)
                result[i][j] = minkowski_distance(centroids[i], centroids[j], 2, false);

    return result;
}

float davies_bouldin_score(vector<Point *> points, vector<int> predicted_clusters)
{
    int n = points.size();

    vector<Point *> centroids = calculate_centroids(points, predicted_clusters);
    vector<float> average_distance_of_points_to_centroids = calculate_average_distance_of_points_to_centroids(
        points, predicted_clusters, centroids);
    vector<vector<float>> distance_between_centroids = calculate_distance_between_centroids(centroids);

    int centroid_count = centroids.size();

    float result = 0.0;
    for (int i = 0; i < centroid_count; i++)
    {
        float max_tmp = 0.0;
        float tmp;
        for (int j = 0; j < centroid_count; j++)
            if (i != j)
            {
                tmp = (average_distance_of_points_to_centroids[i] + average_distance_of_points_to_centroids[j]) / distance_between_centroids[i][j];
                if (max_tmp < tmp)
                    max_tmp = tmp;
            }
        result += max_tmp;
    }

    return result / centroid_count;
}

vector<vector<float>> calculate_average_dist_to_all_points_in_cluster(vector<Point *> points, vector<int> clusters, vector<int> cluster_sizes)
{
    int cluster_count = *max_element(clusters.begin(), clusters.end()) + 1;
    int n = points.size();
    int dim = points[0]->coords.size();

    vector<vector<float>> result(n, vector<float>(cluster_count));

    for (int i = 0; i < n; ++i)
        if (clusters[i] >= 0)
        {
            for (int j = 0; j < n; j++)
                if ((clusters[j] >= 0) && (i != j))
                    result[i][clusters[j]] += minkowski_distance(points[i], points[j], 2, false);
        }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < cluster_count; j++)
        {
            int tmp = clusters[i] == j ? cluster_sizes[j] - 1 : cluster_sizes[j];
            tmp = tmp == 0 ? 1 : tmp;
            result[i][j] = (result[i][j]) / (tmp);
        }

    return result;
}

float silhouette_i(int point, int cluster, int cluster_size, vector<float> average_dist_to_all_points_in_cluster)
{
    if (cluster_size == 1)
        return 0;

    float a = cluster >= 0 ? average_dist_to_all_points_in_cluster[cluster] : 0;
    float b = numeric_limits<float>::max();

    int cluster_count = average_dist_to_all_points_in_cluster.size();

    for (int i = 0; i < cluster_count; i++)
        if (i != cluster)
            if (b > average_dist_to_all_points_in_cluster[i])
                b = average_dist_to_all_points_in_cluster[i];

    float result;
    if (a < b)
        result = 1 - (a / b);
    else if (a == b)
        result = 0;
    else
        result = (b / a) - 1;
    return result;
}

vector<int> calculate_cluster_sizes(vector<int> clusters)
{
    int cluster_count = *max_element(clusters.begin(), clusters.end()) + 1;
    vector<int> cluster_sizes(cluster_count);

    for (int k = 0; k < clusters.size(); k++)
        if (clusters[k] >= 0)
            cluster_sizes[clusters[k]]++;

    return cluster_sizes;
}

float silhouette_coefficient(vector<Point *> points, vector<int> clusters)
{
    //length assert
    int n = points.size();

    vector<int> cluster_sizes = calculate_cluster_sizes(clusters);

    vector<vector<float>> average_dist_to_all_points_in_cluster = calculate_average_dist_to_all_points_in_cluster(points, clusters, cluster_sizes);
    float result = 0.0;
    int points_in_clusters = 0;
    for (int i = 0; i < n; i++)
        if (clusters[i] >= 0)
        {
            result += silhouette_i(i, clusters[i], cluster_sizes[clusters[i]], average_dist_to_all_points_in_cluster[i]);
            points_in_clusters++;
        }

    return result / points_in_clusters;
}
