#include "data.h"

Data load_data(std::string name)
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
        std::ifstream coordinates_file("./datasets/dim512.txt");
        if (coordinates_file.is_open())
        {
            std::string line;
            while (std::getline(coordinates_file, line))
            {
                std::vector<float> coordinats;
                std::stringstream iss(line);
                float number;
                while (iss >> number)
                    coordinats.push_back(number);

                res.points.push_back(coordinats);
            }
            coordinates_file.close();
        }

        std::ifstream labels_file("./datasets/dim512.pa");
        if (labels_file.is_open())
        {
            std::string line;
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
        std::string path;
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
            std::string line;
            while (std::getline(file, line))
            {
                if (line == "@DATA" | line == "@data")
                    break;
            }

            while (std::getline(file, line))
            {
                std::vector<float> coordinats;
                std::istringstream line_stream(line);
                std::string token;

                for (int i = 0; i < dim; ++i)
                {
                    std::getline(line_stream, token, ',');
                    float v = atof(token.c_str());
                    coordinats.push_back(v);
                }
                std::getline(line_stream, token, ',');

                int label;
                if (token == "noise")
                    label = -1;
                else if (isalpha(token[0]))
                    label = token[0] - 'A';
                else
                    label = stoi(token);

                res.points.push_back(coordinats);
                res.labels.push_back(label);
            }
            file.close();
        }
    }
    return res;
}