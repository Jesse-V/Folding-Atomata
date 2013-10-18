
/******************************************************************************\
                     This file is part of Folding Atomata,
          a program that displays 3D views of Folding@home proteins.

                      Copyright (c) 2013, Jesse Victors

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see http://www.gnu.org/licenses/

                For information regarding this software email:
                                Jesse Victors
                         jvictors@jessevictors.com
\******************************************************************************/

#include "Options.hpp"
#include "PyON/StringManip.hpp"
#include <algorithm>
#include <sstream>
#include <iostream>


Options* Options::singleton_ = 0;
Options& Options::getInstance()
{
    if (!singleton_)
        singleton_ = new Options();
    return *singleton_;
}



Options::Options() :
    connectionHost_("127.0.0.1"), connectionPort_(36330), animationDelay_(40),
    usesPassword_(false), bounceAnimation_(false), atomStacks_(8), atomSlices_(16)
{}



bool Options::handleFlags(int argc, char** argv)
{
    std::vector<std::string> options;
    for (int j = 0; j < argc; j++)
        options.push_back(std::string(argv[j]));

    std::size_t index = 1; //skips the name-of-program argument
    while (index < (std::size_t)argc)
    {
        if (options[index] == "--help" || options[index] == "-h")
        {
            std::cout <<
                R".(
Usage:
    FAHViewer [OPTION...]

Commands:
    --animationDelay, -ad  Milliseconds to wait between each animation frame.
    --connect, -c          Address and port to use to connect to FAHClient.
    --bounce, -b           Animation runs backwards at end, like FAHViewer.
    --help, -h             Show flag options and their usage.
    --license              Prints license information.
    --mode, -m             Rendering mode. 3 is stick. Ball-n-stick by default.
    --password, -p         Password for accessing the remote FAHClient.
    --slices, -sl          Slices to use for the atom mesh. Default is 8.
    --stacks, -st          Stacks to use for the atom mesh. Default is 16.
    --verbose, -v          Verbose printing to stdout.
    --version              Print version information.

Examples:
    FoldingAtomata
    FoldingAtomata --connect=203.0.113.0:36330 --password=example
).";
            return false;
        }

        if (options[index] == "--version")
        {
            std::cout << "0.6.8.0" << std::endl;
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
        cycleSnapshots1(flag) || password1(flag) || renderMode1(flag) ||
        atomStacks1(flag) || atomSlices1(flag) || animationDelay1(flag)
        // || slotID1(flag)
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
        cycleSnapshots2(flag, arg) || password2(flag, arg) ||
        renderMode2(flag, arg) || atomStacks2(flag, arg) ||
        atomSlices2(flag, arg) || animationDelay2(flag, arg)
        // || slotID2(flag, arg)
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
        if (!confirm(parts.size() == 2, flag))
            return false;

        auto parameters = StringManip::explode(parts[1], ':');
        if (!confirm(parameters.size() == 2, flag))
            return false;

        connectionHost_ = parameters[0];
        std::istringstream(parameters[1]) >> connectionPort_;
        return true;
    }

    return false;
}



bool Options::connect2(const std::string& flag, const std::string& arg)
{
    if (flag == "--connect" || flag == "-c")
    {
        auto tokens = StringManip::explode(arg, ':');
        if (!confirm(tokens.size() == 2, flag))
            return false;

        connectionHost_ = tokens[0];
        std::istringstream(tokens[1]) >> connectionPort_;
        return true;
    }

    return false;
}



bool Options::bounceSnapshots1(const std::string& flag)
{
    if (StringManip::startsWith(flag, "--bounce-snapshots="))
    {
        auto parts = StringManip::explode(flag, '=');
        if (!confirm(parts.size() == 2, flag))
            return false;

        if (!confirm(parts[1] == "true" || parts[1] == "false", flag))
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
        if (!confirm(next == "true" || next == "false", flag))
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
        if (!confirm(parts.size() == 2, flag))
            return false;

        if (!confirm(parts[1] == "true" || parts[1] == "false", flag))
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
        if (!confirm(next == "true" || next == "false", flag))
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
        if (!confirm(parts.size() == 2, flag))
            return false;

        authPassword_ = StringManip::trim(parts[1], "\"");
        usesPassword_ = true;
        return true;
    }

    return false;
}



bool Options::password2(const std::string& flag, const std::string& arg)
{
    if (flag == "--password" || flag == "-p")
    {
        authPassword_ = StringManip::trim(arg, "\"");
        usesPassword_ = true;
        return true;
    }

    return false;
}



bool Options::renderMode1(const std::string& flag)
{
    if (StringManip::startsWith(flag, "--mode="))
    {
        auto parts = StringManip::explode(flag, '=');
        if (!confirm(parts.size() == 2, flag))
            return false;

        if (parts[1] == "4")
            renderMode_ = RenderMode::BALL_N_STICK;
        else if (parts[1] == "3")
            renderMode_ = RenderMode::STICK;
        else
            renderMode_ = RenderMode::BALL_N_STICK;

        return true;
    }

    return false;
}



bool Options::renderMode2(const std::string& flag, const std::string& arg)
{
    if (flag == "--mode" || flag == "-m")
    {
        if (arg == "4")
            renderMode_ = RenderMode::BALL_N_STICK;
        else if (arg == "3")
            renderMode_ = RenderMode::STICK;
        else
            renderMode_ = RenderMode::BALL_N_STICK;

        return true;
    }

    return false;
}



bool Options::animationDelay1(const std::string& flag)
{
    if (StringManip::startsWith(flag, "--animationDelay="))
    {
        auto parts = StringManip::explode(flag, '=');
        if (!confirm(parts.size() == 2, flag))
            return false;

        std::istringstream(parts[1]) >> animationDelay_;
        return true;
    }

    return false;
}



bool Options::animationDelay2(const std::string& flag, const std::string& arg)
{
    if (flag == "--animationDelay" || flag == "-ad")
    {
        std::istringstream(arg) >> animationDelay_;
        return true;
    }

    return false;
}


/*

bool Options::slotID1(const std::string& flag)
{
    if (StringManip::startsWith(flag, "--slot="))
    {
        auto parts = StringManip::explode(flag, '=');
        if (!confirm(parts.size() == 2, flag))
            return false;

        std::istringstream(parts[1]) >> slotID_;
        slotIDisSet_ = true;
        return true;
    }

    return false;
}



bool Options::slotID2(const std::string& flag, const std::string& arg)
{
    if (flag == "--slot" || flag == "-s")
    {
        std::istringstream(arg) >> slotID_;
        slotIDisSet_ = true;
        return true;
    }

    return false;
}

*/



bool Options::atomStacks1(const std::string& flag)
{
    if (StringManip::startsWith(flag, "--stacks="))
    {
        auto parts = StringManip::explode(flag, '=');
        if (!confirm(parts.size() == 2, flag))
            return false;

        std::istringstream(parts[1]) >> atomStacks_;
        return true;
    }

    return false;
}



bool Options::atomStacks2(const std::string& flag, const std::string& arg)
{
    if (flag == "--stacks" || flag == "-st")
    {
        std::istringstream(arg) >> atomStacks_;
        return true;
    }

    return false;
}



bool Options::atomSlices1(const std::string& flag)
{
    if (StringManip::startsWith(flag, "--slices="))
    {
        auto parts = StringManip::explode(flag, '=');
        if (!confirm(parts.size() == 2, flag))
            return false;

        std::istringstream(parts[1]) >> atomSlices_;
        return true;
    }

    return false;
}



bool Options::atomSlices2(const std::string& flag, const std::string& arg)
{
    if (flag == "--slices" || flag == "-sl")
    {
        std::istringstream(arg) >> atomSlices_;
        return true;
    }

    return false;
}



bool Options::confirm(bool condition, const std::string& flag)
{
    if (!condition)
    {
        std::cerr << "Invalid parameters for " << flag << std::endl;
        return false;
    }

    return true;
}



bool Options::highVerbosity()
{
    return highVerbosity_;
}



std::string Options::getHost()
{
    return connectionHost_;
}



int Options::getPort()
{
    return connectionPort_;
}



std::string Options::getPassword()
{
    return authPassword_;
}


/*
bool Options::slotIDisSet()
{
    return slotIDisSet_;
}



int Options::getSlotID()
{
    return slotID_;
}
*/



unsigned int Options::getAtomStacks()
{
    return atomStacks_;
}



unsigned int Options::getAtomSlices()
{
    return atomSlices_;
}



int Options::getAnimationDelay()
{
    return animationDelay_;
}



bool Options::bounceSnapshots()
{
    return bounceSnapshots_;
}



bool Options::cycleSnapshots()
{
    return cycleSnapshots_;
}



bool Options::usesPassword()
{
    return usesPassword_;
}



Options::RenderMode Options::getRenderMode()
{
    return renderMode_;
}


bool Options::bounceAnimation()
{
    return bounceAnimation_;
}


/* todo:
--help [string] Print help screen or help on a particular option and exit.
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
