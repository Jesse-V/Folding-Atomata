
#include "Options.hpp"
#include "PyON/StringManip.hpp"
#include <algorithm>
#include <sstream>
#include <iostream>


bool Options::handleFlags(int argc, char** argv)
{
    std::vector<std::string> options;
    for (int j = 0; j < argc; j++)
        options.push_back(std::string(argv[j]));

    int index = 0;
    while (index < argc)
    {
        if (options[index] == "--version")
        {
            std::cout << "0.5.1.1" << std::endl;
            return false;
        }  
        
        if (options[index] == "--license")
        {
            std::cout << "GPLv3+" << std::endl;
            return false;
        }

        index += getInstance().handle(options, index);
    }
    
    return true;
}



int Options::handle(const StringList& options, int index)
{
    std::string current = options[index];

    //test for verbosity flags
    if (current == "--verbose" || current == "-v")
    {
        highVerbosity_ = true;
        return 1;
    }

    //test for connect flag, two-piece format
    if (current == "--connect" || current == "-v")
    {
        if (!canGrabNextOption(options, index))
            return 1; //at end of list
        
        auto tokens = StringManip::explode(options[index + 1], ':');
        if (!assert(tokens.size() < 2, options, index))
            return 1;

        connectionIP_   = tokens[0];
        connectionPort_ = tokens[1];
        return 2;
    }

    //test for connect flag, one-piece format
    if (StringManip::startsWith(current, "--connect="))
    {
        auto parts = StringManip::explode(options[index], '=');
        if (!assert(parts.size() < 2, options, index))
            return 1;

        auto parameters = StringManip::explode(parts[1], ':');
        if (!assert(parameters.size() < 2, options, index))
            return 1;

        connectionIP_   = parameters[0];
        connectionPort_ = parameters[1];
        return 1;
    }

    //test for bounce-snapshot flag, two-piece format
    if (current == "--bounce-snapshots" || current == "-b")
    {
        if (!canGrabNextOption(options, index))
            return 1; //at end of list

        std::string next(options[index + 1]);
        if (!assert(next == "true" || next == "false", options, index))
        {
            bounceSnapshots_ = true; //just the flag was given
            return 2;
        }
        
        std::istringstream(next) >> bounceSnapshots_;
        return 2;
    }

    //test for bounce-snapshot flag, once-piece format
    if (StringManip::startsWith(current, "--bounce-snapshots="))
    {
        auto parts = StringManip::explode(options[index], '=');
        if (!assert(parts.size() < 2, options, index))
            return 1;

        if (!assert(parts[1] == "true" || parts[1] == "false", options, index))
            return 1;

        std::istringstream(parts[1]) >> bounceSnapshots_;
        return 1;
    }

    //test for cycle-snapshot flag, two-piece format
    if (current == "--bounce-snapshots" || current == "-b")
    {
        if (!canGrabNextOption(options, index))
            return 1; //at end of list

        std::string next(options[index + 1]);
        if (!assert(next == "true" || next == "false", options, index))
        {
            cycleSnapshots_ = true; //just the flag was given
            return 2;
        }
        
        std::istringstream(next) >> cycleSnapshots_;
        return 2;
    }

    //test for cycle-snapshot flag, once-piece format
    if (StringManip::startsWith(current, "--bounce-snapshots="))
    {
        auto parts = StringManip::explode(options[index], '=');
        if (!assert(parts.size() < 2, options, index))
            return 1;

        if (!assert(parts[1] == "true" || parts[1] == "false", options, index))
            return 1;

        std::istringstream(parts[1]) >> cycleSnapshots_;
        return 1;
    }

    std::cerr << "Unrecognized flag " << options[index] << ". Ignoring." << std::endl;
    return 1;
}



bool Options::canGrabNextOption(const StringList& options, int index)
{
    if (index + 1 >= options.size())
    {
        std::cerr << "Cannot grab argument for " << options[index] << ". " <<
            "Ignoring flag." << std::endl;
        return true;
    }

    return false;
}


bool Options::assert(bool condition, const StringList& options, int index)
{
    if (!condition)
    {
        std::cerr << "Invalid parameters for " << options[index] << std::endl;
        return false;
    }
    
    return true; 
}


Options* Options::singleton_ = 0;

Options& Options::getInstance()
{
    if (!singleton_)
        singleton_ = new Options();
    return *singleton_;
}

/* todo:
--help [string] Print help screen or help on a particular option and exit.
*--connect < string=127.0.0.1:36330 > An address and port to connect to in the form :
*--bounce-snapshots < boolean=false > When the last snapshot is reached, should the animation run backwards or not.
*--cycle-snapshots < boolean=true > Iterate through all available snapshots.
--password < string= > A password for accessing the remote client.
*/

/*
The following options can be specified in a configuration file or on the command
line using the following syntax:
    --<option> <value>
  or:
    --<option>=<value>
  or when marking boolean values true:
    --<option>
*/

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

// ./FoldingAtomata --width=800 --height=600 --mode=4 --cycle-snapshots=true --connect=myIP:36330 --password="examplePassword" --slot=1
