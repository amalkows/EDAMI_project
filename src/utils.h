#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cmath>
#include "data.h"

using namespace std;

float minkowski_distance(Point *a, Point *b, int n = 2, bool update_stats = true);
float get_from_distance_point_matrix(vector<vector<float>> *distance_points_matrix, int a, int b);

#endif
