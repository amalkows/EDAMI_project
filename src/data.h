#ifndef DATA_H
#define DATA_H

#include <vector>
#include <string>

using namespace std;

class Data
{
public:
    vector<vector<float>> points;
    vector<int> labels;
};

Data load_data(string name);

#endif