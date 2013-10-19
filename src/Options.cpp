
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
    highVerbosity_(true), cycleSnapshots_(true), skyboxDisabled_(false),
    connectionPath_("127.0.0.1:36330"),
    imageApath_("/usr/share/FoldingAtomata/images/MSM.png"),
    imageBpath_("/usr/share/FoldingAtomata/images/Primase.png"),
    imageCpath_("/usr/share/FoldingAtomata/images/Ribosome.png"),
    atomStacks_(8), atomSlices_(16), animationDelay_(40)
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
    --cycle-snapshots      If enabled, the animation runs backwards at end.
    --help, -h             Show flag options and their usage.
    --image-a, -ia         Specifies the path to image A for the skybox.
    --image-b, -ib         Specifies the path to image B for the skybox.
    --image-c, -ic         Specifies the path to image C for the skybox.
    --license              Prints license information.
    --mode, -m             Rendering mode. 3 is stick. Ball-n-stick by default.
    --no-skybox            Disables the skybox, leaving a black background.
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
    if (parseBool(flag, "--verbosity", highVerbosity_) ||
        parseBool(flag, "1", "--verbosity", "-v", highVerbosity_) ||
        parseBool(flag, "--cycle-snapshots", cycleSnapshots_) ||
        parseBool(flag, "1", "--cycle-snapshots", "", cycleSnapshots_) ||
        parseBool(flag, "--no-skybox", skyboxDisabled_) ||
        parseBool(flag, "1", "--no-skybox", "", skyboxDisabled_) ||
        parseStr(flag, "--connect", connectionPath_) ||
        parseStr(flag, "--password", authPassword_) ||
        parseStr(flag, "--image-a", imageApath_) ||
        parseStr(flag, "--image-b", imageBpath_) ||
        parseStr(flag, "--image-c", imageCpath_) ||
        parseUInt(flag, "--stacks", atomStacks_) ||
        parseUInt(flag, "--slices", atomSlices_) ||
        parseInt(flag, "--animation-delay", animationDelay_) ||
        renderMode1(flag))
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
    if (parseBool(flag, arg, "--verbosity", "-v", highVerbosity_) ||
        parseBool(flag, arg, "--cycle-snapshots", "", cycleSnapshots_) ||
        parseBool(flag, arg, "--no-skybox", "", skyboxDisabled_) ||
        parseStr(flag, arg, "--connect", "-c", connectionPath_) ||
        parseStr(flag, arg, "--password", "-p", authPassword_) ||
        parseStr(flag, arg, "--image-a", "-ia", imageApath_) ||
        parseStr(flag, arg, "--image-b", "-ib", imageBpath_) ||
        parseStr(flag, arg, "--image-c", "-ic", imageCpath_) ||
        parseUInt(flag, arg, "--stacks", "-st", atomStacks_) ||
        parseUInt(flag, arg, "--slices", "-sl", atomSlices_) ||
        parseInt(flag, arg, "--animation-delay", "-ad", animationDelay_) ||
        renderMode2(flag, arg))
        return 2;

    std::cerr << "Unrecognized flag " << options[index] << ". Ignoring." << std::endl;
    return 1;
}


#define PARSE_SINGLE_FLAG                               \
    if (StringManip::startsWith(flag, target))          \
    {                                                   \
        auto parts = StringManip::explode(flag, '=');   \
        if (!confirm(parts.size() == 2, flag))          \
            return false;                               \
                                                        \
        std::istringstream(parts[1]) >> param;          \
        return true;                                    \
    }                                                   \
    return false;


#define PARSE_DOUBLE_FLAG                               \
    if (flag == target1 || flag == target2)             \
    {                                                   \
        std::istringstream(arg) >> param;               \
        return true;                                    \
    }                                                   \
    return false;

bool Options::parseBool(StrRef flag, StrRef target, bool& param)
{ PARSE_SINGLE_FLAG }


bool Options::parseBool(StrRef flag, StrRef arg, StrRef target1, StrRef target2,
                                                        bool& param)
{ PARSE_DOUBLE_FLAG }


bool Options::parseStr(StrRef flag, StrRef target, std::string& param)
{ PARSE_SINGLE_FLAG }


bool Options::parseStr(StrRef flag, StrRef arg, StrRef target1, StrRef target2,
                                                std::string& param)
{ PARSE_DOUBLE_FLAG }


bool Options::parseUInt(StrRef flag, StrRef target, unsigned int& param)
{ PARSE_SINGLE_FLAG }


bool Options::parseUInt(StrRef flag, StrRef arg, StrRef target1, StrRef target2,
                                                unsigned int& param)
{ PARSE_DOUBLE_FLAG }


bool Options::parseInt(StrRef flag, StrRef target, int& param)
{ PARSE_SINGLE_FLAG }


bool Options::parseInt(StrRef flag, StrRef arg, StrRef target1, StrRef target2,
                                                            int& param)
{ PARSE_DOUBLE_FLAG }



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



bool Options::confirm(bool condition, const std::string& flag)
{
    if (!condition)
    {
        std::cerr << "Invalid parameters for " << flag << std::endl;
        return false;
    }

    return true;
}



std::string Options::getHost()
{
    return StringManip::explode(connectionPath_, ':')[0];
}



int Options::getPort()
{
    int port;
    std::istringstream(StringManip::explode(connectionPath_, ':')[1]) >> port;
    return port;
}



bool Options::usesPassword()
{
    return !authPassword_.empty();
}



std::string Options::getPassword()
{
    return authPassword_;
}



Options::RenderMode Options::getRenderMode()
{
    return renderMode_;
}



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



bool Options::cycleSnapshots()
{
    return cycleSnapshots_;
}



bool Options::highVerbosity()
{
    return highVerbosity_;
}



bool Options::skyboxDisabled()
{
    return skyboxDisabled_;
}



std::string Options::getPathToImageA()
{
    return imageApath_;
}



std::string Options::getPathToImageB()
{
    return imageBpath_;
}



std::string Options::getPathToImageC()
{
    return imageCpath_;
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

// ./FoldingAtomata --width=800 --height=600 --mode=4 --cycle-snapshots=true --connect=myIP:36330 --password="examplePassword" --slot=1
