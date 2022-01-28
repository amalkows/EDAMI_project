#include "globals.h"

std::string out_name = "";
std::string stat_name = "";
std::string debug_name = "";

void write_to_file(std::string path, std::string text)
{
    std::ofstream myfile;
    myfile.open(path);
    myfile << text;
    myfile.close();
}
