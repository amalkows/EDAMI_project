#include "utils.h"

float minkowski_distance(vector<float> a, vector<float> b, int n)
{
    float result = 0;

    for (int i = 0; i < a.size(); i++)
        result += pow(a[i] - b[i], n);

    return pow(result, 1.0 / n);
}
