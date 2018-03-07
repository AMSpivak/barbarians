#ifndef LOADER_UTILITY
#define LOADER_UTILITY
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

namespace LoaderUtility
{
    std::string FindPrefix(std::ifstream &file);
    void LoadLineBlock(std::ifstream &file,const std::string &sufix,std::vector<std::string> &lines);
}

#endif