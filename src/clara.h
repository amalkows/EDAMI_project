#ifndef CLARA_H
#define CLARA_H

#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <limits>
#include <tuple>

#include "utils.h"

using namespace std;

vector<int> clara(
    vector<vector<float>> points,
    int cluster_count,
    int random_group_count = 10,
    int random_group_size = 100,
    int minkowski_n = 2,
    int max_swap = 500,
    bool multithread = false,
    bool optimized_pam_init = false);
vector<int> pam(vector<vector<float>> points, int cluster_count, int minkowski_n = 2, int max_swap = 500, bool optimized_pam_ini = false);

vector<vector<float>> draw_data(vector<vector<float>> points, int random_group_size);
vector<vector<int>> init_centroids(vector<vector<float>> points, int cluster_count);
tuple<float, float> calculate_d_j(vector<vector<float>> distance_points_matrix, vector<int> centroids, int j);

#endif