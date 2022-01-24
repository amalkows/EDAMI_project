#ifndef DATA_H
#define DATA_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>

using namespace std;

class Data
{
public:
    vector<vector<float>> points;
    vector<int> labels;
};

Data load_data(string name);

#endif