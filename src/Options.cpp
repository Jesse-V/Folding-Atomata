
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
#include <tclap/CmdLine.h>
#include <stdexcept>


Options* Options::singleton_ = 0;
Options& Options::getInstance()
{
    if (!singleton_)
        singleton_ = new Options();
    return *singleton_;
}



bool Options::handleFlags(int argc, char** argv)
{
    static bool called = false;
    if (called)
        throw std::runtime_error("HandleFlags called multiple times!");

    called = true;
    return Options::getInstance().handleFlagsInternal(argc, argv);
}



bool Options::handleFlagsInternal(int argc, char** argv)
{
    TCLAP::ValueArg<unsigned int> animationDelayFlag("a", "animation-delay",
        "Milliseconds to wait between each animation frame.", false,
        40, "long");

    TCLAP::ValueArg<std::string> connectFlag("c", "connect",
        "Address and port to use to connect to FAHClient.", false,
        "127.0.0.1:36330", "IP:port");

    TCLAP::SwitchArg cycleSnapshotsFlag("C", "cycle-snapshots",
        "If enabled, the animation runs backwards at end.", false);

    TCLAP::ValueArg<std::string> skyboxImageFlag("i", "image",
        "Specifies the path to image for the skybox.", false,
        "/usr/share/FoldingAtomata/images/gradient.png", "path");

    TCLAP::SwitchArg licenseFlag("l", "license",
        "Prints license information and exits.", false);

    TCLAP::ValueArg<unsigned int> modeFlag("m", "mode",
        "Rendering mode. 3 is stick. Ball-n-stick by default.", false,
        0, "milliseconds");

    TCLAP::SwitchArg noSkyboxFlag("n", "no-skybox",
        "Disables the skybox, leaving a black background.", false);

    TCLAP::SwitchArg oneSlotFlag("o", "one-slot",
        "Only shows one slot, instead of all available slots.", false);

    TCLAP::ValueArg<std::string> passwordFlag("p", "password",
        "Password for accessing the remote FAHClient.", false,
        "", "string");

    TCLAP::ValueArg<unsigned int> slicesFlag("s", "slices",
        "Slices to use for the atom mesh. Default is 8.", false,
        8, "unsigned int");

    TCLAP::ValueArg<unsigned int> stacksFlag("S", "stacks",
        "Stacks to use for the atom mesh. Default is 16.", false,
        16, "unsigned int");

    TCLAP::SwitchArg verboseFlag("v", "verbose", //this could be a MultiSwitch
        "Verbose printing to stdout.", false);

    TCLAP::CmdLine cmd(R".(Examples:
        FoldingAtomata
        FoldingAtomata --connect=203.0.113.0:36330 --password=example
        ).", '=', "1.4.1.0");
    cmd.add(animationDelayFlag);
    cmd.add(connectFlag);
    cmd.add(cycleSnapshotsFlag);
    cmd.add(skyboxImageFlag);
    cmd.add(licenseFlag);
    cmd.add(modeFlag);
    cmd.add(noSkyboxFlag);
    cmd.add(oneSlotFlag);
    cmd.add(passwordFlag);
    cmd.add(slicesFlag);
    cmd.add(stacksFlag);
    cmd.add(verboseFlag);

    cmd.parse(argc, argv);

    animationDelay_ = animationDelayFlag.getValue();
    connectionPath_ = connectFlag.getValue();
    cycleSnapshots_ = cycleSnapshotsFlag.isSet();
    imagePath_ = skyboxImageFlag.getValue();

    if (licenseFlag.isSet())
    {
        std::cout << "GPLv3+" << std::endl;
        return false;
    }

    if (modeFlag.isSet())
    {
        switch (modeFlag.getValue())
        {
            case 4:
                renderMode_ = RenderMode::BALL_N_STICK;
                break;
            case 3:
                renderMode_ = RenderMode::STICK;
                break;
            default:
                renderMode_ = RenderMode::BALL_N_STICK;
        }
    }

    skyboxDisabled_ = noSkyboxFlag.isSet();
    oneSlot_        = oneSlotFlag.isSet();
    authPassword_   = passwordFlag.getValue();
    atomSlices_     = slicesFlag.getValue();
    atomStacks_     = stacksFlag.getValue();
    highVerbosity_  = verboseFlag.isSet();

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
    return (int)animationDelay_;
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



std::string Options::getSkyboxPath()
{
    return imagePath_;
}



bool Options::showOneSlot()
{
    return oneSlot_;
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
