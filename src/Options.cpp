
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

    std::size_t index = 1; //skips the name-of-program argument
    while (index < (std::size_t)argc)
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



std::size_t Options::handle(const StringList& options, std::size_t index)
{
    std::string flag = options[index];

    //check for 1-piece flags
    if (verbose1(flag) || connect1(flag) || bounceSnapshots1(flag) ||
        cycleSnapshots1(flag) || password1(flag) || slotID1(flag)
    )
        return 1;

    //check to see if we can grab next flag
    if (index + 1 >= options.size())
    {
        std::cerr << "Unrecognized flag or unavailable argument for " << 
            flag << ". Ignoring." << std::endl;
        return 2;
    }

    //check for two-piece flags
    std::string arg(options[index + 1]);
    if (connect2(flag, arg) || bounceSnapshots2(flag, arg) ||
        cycleSnapshots2(flag, arg) || password2(flag, arg) || slotID2(flag, arg)
    )
        return 2;

    std::cerr << "Unrecognized flag " << options[index] << ". Ignoring." << std::endl;
    return 1;
}



bool Options::verbose1(const std::string& flag)
{
    //test for verbosity flags
    if (flag == "--verbose" || flag == "-v")
    {
        highVerbosity_ = true;
        return true;
    }

    return false;
}



bool Options::connect1(const std::string& flag)
{
    if (StringManip::startsWith(flag, "--connect="))
    {
        auto parts = StringManip::explode(flag, '=');
        if (!assert(parts.size() == 2, flag))
            return false;

        auto parameters = StringManip::explode(parts[1], ':');
        if (!assert(parameters.size() == 2, flag))
            return false;

        connectionIP_   = parameters[0];
        connectionPort_ = parameters[1];
        return true;
    }

    return false;
}



bool Options::connect2(const std::string& flag, const std::string& arg)
{
    if (flag == "--connect" || flag == "-v")
    {
        auto tokens = StringManip::explode(arg, ':');
        if (!assert(tokens.size() == 2, flag))
            return false;

        connectionIP_   = tokens[0];
        connectionPort_ = tokens[1];
        return true;
    }

    return false;
}



bool Options::bounceSnapshots1(const std::string& flag)
{
    if (StringManip::startsWith(flag, "--bounce-snapshots="))
    {
        auto parts = StringManip::explode(flag, '=');
        if (!assert(parts.size() == 2, flag))
            return false;

        if (!assert(parts[1] == "true" || parts[1] == "false", flag))
            return false;

        std::istringstream(parts[1]) >> bounceSnapshots_;
        return true;
    }

    return false;
}



bool Options::bounceSnapshots2(const std::string& flag, const std::string& arg)
{
    if (flag == "--bounce-snapshots" || flag == "-b")
    {
        std::string next(arg);
        if (!assert(next == "true" || next == "false", flag))
        {
            bounceSnapshots_ = true; //just the flag was given
            return true;
        }
        
        std::istringstream(next) >> bounceSnapshots_;
        return true;
    }

    return false;
}



bool Options::cycleSnapshots1(const std::string& flag)
{
    if (StringManip::startsWith(flag, "--cycle-snapshots="))
    {
        auto parts = StringManip::explode(flag, '=');
        if (!assert(parts.size() == 2, flag))
            return false;

        if (!assert(parts[1] == "true" || parts[1] == "false", flag))
            return false;

        std::istringstream(parts[1]) >> cycleSnapshots_;
        return true;
    }

    return false;
}



bool Options::cycleSnapshots2(const std::string& flag, const std::string& arg)
{
    if (flag == "--cycle-snapshots" || flag == "-b")
    {
        std::string next(arg);
        if (!assert(next == "true" || next == "false", flag))
        {
            cycleSnapshots_ = true; //just the flag was given
            return false;
        }
        
        std::istringstream(next) >> cycleSnapshots_;
        return true;
    }

    return false;
}



bool Options::password1(const std::string& flag)
{
    if (StringManip::startsWith(flag, "--password="))
    {
        auto parts = StringManip::explode(flag, '=');
        if (!assert(parts.size() == 2, flag))
            return false;

        authPassword_ = StringManip::trim(parts[1], "\"");
        return true;
    }

    return false;
}



bool Options::password2(const std::string& flag, const std::string& arg)
{
    if (flag == "--password" || flag == "-p")
    {
        authPassword_ = StringManip::trim(arg, "\"");
        return true;
    }

    return false;
}



bool Options::slotID1(const std::string& flag)
{
    if (StringManip::startsWith(flag, "--slot="))
    {
        auto parts = StringManip::explode(flag, '=');
        if (!assert(parts.size() == 2, flag))
            return false;

        std::istringstream(parts[1]) >> slotID_;
        return true;
    }

    return false;
}



bool Options::slotID2(const std::string& flag, const std::string& arg)
{
    if (flag == "--slot" || flag == "-s")
    {
        std::istringstream(arg) >> slotID_;
        return true;
    }

    return false;
}



bool Options::assert(bool condition, const std::string& flag)
{
    if (!condition)
    {
        std::cerr << "Invalid parameters for " << flag << std::endl;
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
*--password < string= > A password for accessing the remote client.
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
