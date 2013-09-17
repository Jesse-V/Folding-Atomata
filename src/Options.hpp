
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

typedef std::vector<std::string> StringList;

class Options
{
    public:
        static Options& getInstance();
        static bool handleFlags(int argc, char** argv); //should only call once!

        enum class RenderMode : short
        {
            BALL_N_STICK, STICK
        };

        Options();
        std::string getHost();
        int getPort();
        std::string getPassword();
        unsigned int getAtomStacks();
        unsigned int getAtomSlices();
        //bool slotIDisSet();
        //int getSlotID();

        bool highVerbosity();
        bool bounceSnapshots();
        bool cycleSnapshots();
        bool usesPassword();
        RenderMode getRenderMode();

    private:
        std::size_t handle(const StringList& options, std::size_t index);
        bool verbose1(const std::string& flag);
        bool connect1(const std::string& flag);
        bool connect2(const std::string& flag, const std::string& arg);
        bool bounceSnapshots1(const std::string& flag);
        bool bounceSnapshots2(const std::string& flag, const std::string& arg);
        bool cycleSnapshots1(const std::string& flag);
        bool cycleSnapshots2(const std::string& flag, const std::string& arg);
        bool password1(const std::string& flag);
        bool password2(const std::string& flag, const std::string& arg);
        bool renderMode1(const std::string& flag);
        bool renderMode2(const std::string& flag, const std::string& arg);
        //bool slotID1(const std::string& flag);
        //bool slotID2(const std::string& flag, const std::string& arg);
        bool atomStacks1(const std::string& flag);
        bool atomStacks2(const std::string& flag, const std::string& arg);
        bool atomSlices1(const std::string& flag);
        bool atomSlices2(const std::string& flag, const std::string& arg);
        bool confirm(bool condition, const std::string& flag);

    private:
        static Options* singleton_;

        std::string connectionHost_;
        int connectionPort_; //slotID_, ;
        std::string authPassword_;

        RenderMode renderMode_ = RenderMode::BALL_N_STICK;
        bool highVerbosity_, bounceSnapshots_, cycleSnapshots_, usesPassword_;
        //, slotIDisSet_;
        unsigned int atomStacks_, atomSlices_;
};

#endif
