
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

#ifndef OPTIONS
#define OPTIONS

#include <vector>
#include <string>

typedef const std::string& StrRef;
typedef std::vector<std::string> StringList;

class Options
{
    public:
        static Options& getInstance();
        static bool handleFlags(int argc, char** argv);

        enum class RenderMode : short
        {
            BALL_N_STICK, STICK
        };

        std::string getHost();
        int getPort();
        bool usesPassword();
        std::string getPassword();
        RenderMode getRenderMode();
        unsigned int getAtomStacks();
        unsigned int getAtomSlices();
        int getAnimationDelay();
        bool cycleSnapshots();
        bool highVerbosity();
        bool skyboxDisabled();
        std::string getSkyboxPath();
        bool showOneSlot();

    private:
        bool handleFlagsInternal(int argc, char** argv);

    private:
        static Options* singleton_;

        bool highVerbosity_, cycleSnapshots_, skyboxDisabled_, oneSlot_;
        std::string connectionPath_, authPassword_, imagePath_;
        unsigned int atomStacks_, atomSlices_, animationDelay_;
        RenderMode renderMode_ = RenderMode::BALL_N_STICK;
};

#endif
