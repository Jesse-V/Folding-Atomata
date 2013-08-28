
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

#include "StringManip.hpp"
#include <algorithm>
#include <sstream>


/*
    Returns the contents of the string between the header and the footer,
    excluding both
*/
Str StringManip::between(const Str& str, const Str& header,
                         const Str& footer, std::size_t start
)
{
    std::size_t head = str.find(header, start);
    std::size_t foot = str.find(footer, head);
    return str.substr(head + header.length(), foot - head - header.length());
}



/*
    Explodes the string around the given delimiter, then trims away
    any of the given whitespace characters from sides of the tokens.
*/
StringVector StringManip::explodeAndTrim(const Str& str, char delim, 
                                         const Str& whitespaces
)
{
    auto tokens = explode(str, delim);
    std::transform(tokens.begin(), tokens.end(), tokens.begin(), 
        [&](const std::string& token)
        {
            return trim(token, whitespaces);
        }
    );

    return tokens;
}



/*
    Explodes the string around the given delimiter.
    (Reproduction of PHP's explode)
*/
StringVector StringManip::explode(const Str& str, char delim)
{
    std::vector<std::string> tokens;
    std::stringstream stream(str);
    std::string item;
    while (std::getline(stream, item, delim))
        tokens.push_back(item);        

    return tokens;
}



/*
    Trims any of the given whitespace characters off of 
    both sides of the given string.
*/
Str StringManip::trim(const Str& str, const Str& whitespaces)
{
    std::size_t start = str.find_first_not_of(whitespaces);
    std::size_t end = str.find_last_not_of(whitespaces);

    if (start != std::string::npos)
        return str.substr(start, end - start + 1);
    return "";
}
