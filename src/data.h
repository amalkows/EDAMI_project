#ifndef DATA_H
#define DATA_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>

class Point
{
public:
    Point()
    {
    }
    Point(int init_index, std::vector<float> init_coords)
    {
        coords = init_coords;
        index = init_index;
    }
    Point(int dim)
    {
        for (int i = 0; i < dim; ++i)
            coords.push_back(0);
    }
    int index;
    std::vector<float> coords;
    int metric_calculate_count = 0;
};

class Data
{
public:
    std::vector<Point *> points;
    std::vector<int> labels;
};

Data load_data(std::string name);

#endif