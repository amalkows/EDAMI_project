#ifndef METRICS_H
#define METRICS_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <limits>
#include "utils.h"

using namespace std;

float pruity_score(vector<int> true_clusters, vector<int> predicted_clusters);
int factorial(int i);
float rand_score(vector<int> true_clusters, vector<int> predicted_clusters);
vector<vector<float>> calculate_centroids(vector<vector<float>> points, vector<int> clusters);
vector<float> calculate_average_distance_of_points_to_centroids(
    vector<vector<float>> points,
    vector<int> clusters,
    vector<vector<float>> centroids);
vector<vector<float>> calculate_distance_between_centroids(vector<vector<float>> centroids);
float davies_bouldin_score(vector<vector<float>> points, vector<int> predicted_clusters);
vector<vector<float>> calculate_average_dist_to_all_points_in_cluster(vector<vector<float>> points, vector<int> clusters, vector<int> cluster_sizes);
float silhouette_i(int point, int cluster, int cluster_size, vector<float> average_dist_to_all_points_in_cluster);
vector<int> calculate_cluster_sizes(vector<int> clusters);
float silhouette_coefficient(vector<vector<float>> points, vector<int> clusters);

#endif
