#ifndef LOADER_UTILITY
#define LOADER_UTILITY
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <functional>

namespace LoaderUtility
{
    std::string FindPrefix(std::ifstream &file);
    void LoadLineBlock(std::ifstream &file,const std::string &sufix,std::vector<std::string> &lines);

    class LinesProcessor
    {
        private:
        std::map<std::string,const std::function<void(std::stringstream&)>> execute_funcs;
        public:
        void Process(std::vector<std::string> &lines);
        void AddProcessFunction(std::string tag,const std::function<void(std::stringstream&)> function);
    };
}

#endif