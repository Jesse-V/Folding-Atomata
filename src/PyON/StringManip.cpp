
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
#include <iostream>

/*
    Returns the contents of the string between the header and the footer,
    excluding both
*/
std::string StringManip::between(StrRef str, StrRef header, StrRef footer,
                                 std::size_t start
)
{
    auto index = between(str, std::make_pair(start, str.size() - 1), header, footer);
    return str.substr(index.first, index.second - index.first);
}



Indexes StringManip::between(StrRef str, IndexesRef indexes,
                             StrRef header, StrRef footer
)
{
    std::size_t head = str.find(header, indexes.first);
    std::size_t foot = str.find(footer, head);
    std::size_t startIndex = head + header.length();
    return std::make_pair(startIndex, foot);
}



/*
    Explodes the string around the given delimiter, then trims away
    any of the given whitespace characters from sides of the tokens.
*/
std::vector<std::string> StringManip::explodeAndTrim(StrRef str, char delim,
                                                     StrRef whitespaces
)
{
    auto tokens = explode(str, delim);
    //std::cout << "explodeTrim: " << tokens[0] << "," << tokens[1] << std::endl;

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
std::vector<std::string> StringManip::explode(StrRef str, char delim)
{
    auto indexes = explode(str, std::make_pair(0, str.size()), delim);
    std::vector<std::string> tokens;
    for (auto pair : indexes)
        tokens.push_back(str.substr(pair.first, pair.second - pair.first + 1));
    return tokens;
}



IndexesVector StringManip::explode(StrRef str, IndexesRef indexes, char delim)
{
    IndexesVector indexesVector;
    std::size_t startIndex = indexes.first;
    while (startIndex < indexes.second)
    {
        std::size_t lastIndex = str.find(delim, startIndex);
        if (lastIndex == std::string::npos)
        {
            indexesVector.push_back(std::make_pair(startIndex, indexes.second - 1));
            break;
        }

        indexesVector.push_back(std::make_pair(startIndex, lastIndex - 1));
        startIndex = lastIndex + 1;
    }

    return indexesVector;
}



/*
    Trims any of the given whitespace characters off of
    both sides of the given string.
*/
std::string StringManip::trim(StrRef str, StrRef whitespaces)
{
    //std::cout << "asked to trim: " << str << std::endl;
    auto index = trim(str, std::make_pair(0, str.size() - 1), whitespaces);
    //std::cout << "trimIs: " << index.first << "()" << index.second << std::endl;
    //std::cout << "Tresult: " << str.substr(index.first, index.second - index.first + 1) << std::endl;
    return str.substr(index.first, index.second - index.first + 1);
}



Indexes StringManip::trim(StrRef str, IndexesRef indexes, StrRef whitespaces)
{
    std::size_t start = str.find_first_not_of(whitespaces, indexes.first);
    std::size_t end = str.find_last_not_of(whitespaces, indexes.second);

    //std::cout << "trim: " << start << "," << end << std::endl;

    if (start != std::string::npos)
        return std::make_pair(start, end);
    //std::cout << "returned blank" << std::endl;
    return std::make_pair(indexes.first, indexes.first - 1);
}



/*
    Tests to see if a begins with b
*/
bool StringManip::startsWith(StrRef a, StrRef b)
{
    return a.compare(0, b.length(), b) == 0;
}
