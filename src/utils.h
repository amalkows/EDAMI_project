#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cmath>
#include "data.h"

using namespace std;

float minkowski_distance(Point *a, Point *b, int n = 2, bool update_stats = true);

#endif
