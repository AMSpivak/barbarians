#include "loader.h"
#include <regex>

namespace LoaderUtility
{

    std::string FindPrefix(std::ifstream &file)
    {
        std::string tempholder("");
        std::regex search_regexp("<([^!][^>]*)>");
        std::smatch match;

        while(!file.eof())
        {
            getline(file, tempholder);
            if (std::regex_search(tempholder, match, search_regexp) && match.size() > 1) 
            {
                tempholder = match.str(1);
                return tempholder;
            }
        }
        return std::string("");
        
    }

    void LoadLineBlock(std::ifstream &file,const std::string &sufix,std::vector<std::string> &lines)
    {
        lines.clear();

        //std::string sufix_start("<"+sufix+">");
        std::string sufix_end("<!"+sufix+">");
        std::string tempholder("");
/*
        while(!file.eof()&&(tempholder.compare(sufix_start)))
        {
            getline(file, tempholder);
            std::cout<<tempholder<<"\n";
        }*/

        std::regex search_regexp("<([!]" + sufix + ")>");
        std::smatch match;

        bool is_block_endline = false;


        while(!file.eof()&&(!is_block_endline))
        {
            getline(file, tempholder);

            if (std::regex_search(tempholder, match, search_regexp) && match.size() > 1) 
            {
                is_block_endline = 1;
                std::cout<<match.str(1)<<"\n";
            }

            if(!is_block_endline)
            {
                lines.push_back(tempholder);
            }
        }
        

    }
}