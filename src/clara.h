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
#include <chrono>

#include "utils.h"
#include "data.h"
#include "globals.h"

using namespace std;
using namespace std::chrono;

tuple<vector<int>, vector<string>, vector<Point *>> clara(
    vector<Point *> points,
    int cluster_count,
    int random_group_count = 10,
    int random_group_size = 100,
    int minkowski_n = 2,
    int max_swap = 500,
    bool multithread = false,
    bool optimized_pam_init = false);
vector<Point *> pam(vector<Point *> points, int cluster_count, int minkowski_n = 2, int max_swap = 500, bool optimized_pam_ini = false);
tuple<float, vector<int>> prepare_clustering_based_on_centroids(vector<Point *> centroids, vector<Point *> points, int minkowski_n = 2);
vector<Point *> draw_data(vector<Point *> points, int random_group_size);
vector<vector<int>> init_centroids(vector<Point *> points, int cluster_count, bool optimized_pam_init);
tuple<float, float> calculate_d_j(vector<vector<float>> *distance_points_matrix, vector<int> centroids, int j);

#endif