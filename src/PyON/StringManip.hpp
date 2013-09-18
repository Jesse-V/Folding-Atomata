
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

#ifndef STRING_MANIP
#define STRING_MANIP

/**
    Defines some useful string manipulation methods. These are used by the
    TrajectoryParser class for isolating data in a PyON-formatted string.
**/

#include <string>
#include <vector>

typedef const std::string& StrRef;
typedef std::pair<std::size_t, std::size_t> Indexes;
typedef const Indexes& IndexesRef;
typedef std::vector<Indexes> IndexesVector;

class StringManip
{
    public:
        static std::string between(StrRef str, StrRef header, StrRef footer,
                                   std::size_t start = 0
        );
        static Indexes between(StrRef str, IndexesRef indexes,
                               StrRef header, StrRef footer
        );
        static std::vector<std::string> explodeAndTrim(StrRef str,
                                            char delim, StrRef whitespaces
        );
        static std::vector<std::string> explode(StrRef str, char delim);
        static IndexesVector explode(StrRef, IndexesRef indexes, char delim);
        static std::string trim(StrRef str, StrRef whitespaces);
        static Indexes trim(StrRef str, IndexesRef indexes, StrRef whitespaces);
        static bool startsWith(StrRef a, StrRef b);
};

#endif
