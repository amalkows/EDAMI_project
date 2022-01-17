#ifndef DBSCAN_H
#define DBSCAN_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <limits>
#include "utils.h"

using namespace std;

vector<int> eps_neighborhood(
    vector<float> ref,
    vector<vector<float>> points,
    float eps,
    int minkowski_n = 2);

vector<int> dbscan(vector<vector<float>> points, float eps, int min_pts, int minkowski_n = 2);

#endif