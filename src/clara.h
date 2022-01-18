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

vector<int> clara(vector<vector<float>> points, int cluster_count, int minkowski_n = 2, int max_swap = 500);
vector<int> pam(vector<vector<float>> points, int cluster_count, int minkowski_n = 2, int max_swap = 500);

vector<vector<int>> init_centroids(vector<vector<float>> points, int cluster_count);
tuple<float, float> calculate_d_j(vector<vector<float>> distance_points_matrix, vector<int> centroids, int j);

#endif