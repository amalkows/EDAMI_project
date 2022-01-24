#ifndef DATA_H
#define DATA_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>

// using namespace std;

class Data
{
public:
    std::vector<std::vector<float>> points;
    std::vector<int> labels;
};

Data load_data(std::string name);

#endif