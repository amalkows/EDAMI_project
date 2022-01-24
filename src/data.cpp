#include "data.h"

Data load_data(string name)
{
    Data res;
    if (name == "toy")
    {
        res.points = {
            {4.2, 4.0},  //A 0
            {5.9, 3.9},  //B 1
            {2.8, 3.5},  //C 2
            {12.0, 1.3}, //D 3
            {10.0, 1.3}, //E 4
            {1.1, 3.0},  //F 5
            {0.0, 2.4},  //G 6
            {2.4, 2.0},  //H 7
            {11.5, 1.8}, //I 8
            {11.0, 1.0}, //J 9
            {0.9, 0.0},  //K 10
            {1.0, 1.5},  //L 11
        };
    }
    else if (name == "dim512")
    {
        ifstream coordinates_file("./datasets/dim512.txt");
        if (coordinates_file.is_open())
        {
            string line;
            while (std::getline(coordinates_file, line))
            {
                vector<float> coordinats;
                stringstream iss(line);
                float number;
                while (iss >> number)
                    coordinats.push_back(number);

                res.points.push_back(coordinats);
            }
            coordinates_file.close();
        }

        ifstream labels_file("./datasets/dim512.pa");
        if (labels_file.is_open())
        {
            string line;
            int i = 0;
            while (getline(labels_file, line))
            {
                ++i;
                if (i < 6)
                    continue;

                res.labels.push_back(stoi(line));
            }
            labels_file.close();
        }
    }
    else
    {
        string path;
        int dim;
        if (name == "cluto-t7-10k")
        {
            path = "./datasets/cluto-t7-10k.arff";
            dim = 2;
        }
        else if (name == "complex9")
        {
            path = "./datasets/complex9.arff";
            dim = 2;
        }
        else if (name == "letter")
        {
            path = "./datasets/letter.arff";
            dim = 16;
        }

        std::ifstream file(path);

        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                if (line == "@DATA" | line == "@data")
                    break;
            }

            while (getline(file, line))
            {
                vector<float> coordinats;
                stringstream iss(line);
                // std::copy(istream_iterator<float>(iss),
                //           std::istream_iterator<float>(),
                //           std::back_inserter(coordinats));
                for (int i = 0; i < dim; ++i)
                {
                    float v = 0;
                    iss >> v;
                    coordinats.push_back(v);
                }
                int label;
                iss >> label;
                res.labels.push_back(label);
                res.points.push_back(coordinats);
            }
            file.close();
        }
    }
    return res;
}