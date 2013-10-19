
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
        static bool handleFlags(int argc, char** argv); //should only call once!

        enum class RenderMode : short
        {
            BALL_N_STICK, STICK
        };

        Options();
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
        std::string getPathToImageA();
        std::string getPathToImageB();
        std::string getPathToImageC();

    private:
        std::size_t handle(const StringList& options, std::size_t index);
        bool parseBool(StrRef flag, StrRef target, bool& param);
        bool parseBool(StrRef flag, StrRef arg, StrRef target1, StrRef target2,
                                                                bool& param);
        bool parseStr(StrRef flag, StrRef target, std::string& param);
        bool parseStr(StrRef flag, StrRef arg, StrRef target1, StrRef target2,
                                                        std::string& param);
        bool parseUInt(StrRef flag, StrRef target, unsigned int& param);
        bool parseUInt(StrRef flag, StrRef arg, StrRef target1, StrRef target2,
                                                        unsigned int& param);
        bool parseInt(StrRef flag, StrRef target, int& param);
        bool parseInt(StrRef flag, StrRef arg, StrRef target1, StrRef target2,
                                                                    int& param);
        bool renderMode1(const std::string& flag);
        bool renderMode2(const std::string& flag, const std::string& arg);
        bool confirm(bool condition, const std::string& flag);

    private:
        static Options* singleton_;

        bool highVerbosity_, cycleSnapshots_, skyboxDisabled_;
        std::string connectionPath_, authPassword_, imageApath_, imageBpath_,
            imageCpath_;
        unsigned int atomStacks_, atomSlices_;
        int animationDelay_;
        RenderMode renderMode_ = RenderMode::BALL_N_STICK;
};

#endif
