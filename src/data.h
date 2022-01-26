#ifndef DATA_H
#define DATA_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>

// using namespace std;

class Point
{
public:
    Point()
    {
    }
    // Point(const Point &p)
    // {
    //     coords = p.coords;
    //     metric_calculate_count = p.metric_calculate_count;
    // }
    Point(int init_index, std::vector<float> init_coords)
    {
        coords = init_coords;
        index = init_index;
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