#ifndef DBSCAN_H
#define DBSCAN_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <limits>
#include <tuple>
#include <chrono>

#include "utils.h"
#include "data.h"
#include "globals.h"

using namespace std;
using namespace std::chrono;

vector<int> eps_neighborhood(int ref, float eps, vector<vector<float>> *distance_points_matrix);

tuple<vector<int>, vector<string>, vector<int>> dbscan(vector<Point *> points, float eps, int min_pts, int minkowski_n = 2);

#endif