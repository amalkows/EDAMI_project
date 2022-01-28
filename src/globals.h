#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <iostream>
#include <fstream>

extern std::string out_name, stat_name, debug_name;

void write_to_file(std::string path, std::string text);

#endif
