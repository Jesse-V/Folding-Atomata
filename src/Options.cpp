
#include "Options.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

bool Options::highVerbosity_;

bool Options::handleFlags(int argc, char** argv)
{
    std::vector<std::string> options;
    for (int j = 0; j < argc; j++)
        options.push_back(std::string(argv[j]));

    for (int j = 0; j < argc; j++)
    {
        std::string option(argv[j]);

        if (option == "--version")
        {
            std::cout << "0.5.1.1" << std::endl;
            return false;
        }  
        else if (option == "--license")
        {
            std::cout << "GPLv3+" << std::endl;
            return false;
        }
        else if (option == "--v" || option == "-v")
        {
            highVerbosity_ = true;
        }
    }
    
    return true;
}

/*
**FoldingAtomata**
**--width=800**
**--height=600**
**--mode=4**
**--cycle-snapshots=true**
**--connect=myIP:36330**
**--password="examplePassword"**
**--slot=1**
*/
