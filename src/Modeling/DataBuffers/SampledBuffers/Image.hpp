
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

#ifndef IMAGE
#define IMAGE

#include <string>

class Image
{
    public:
        Image(const std::string& imagePath);
        int getWidth();
        int getHeight();
        unsigned char* getImageData();        

    protected:
        void loadBMP(const std::string& imagePath);
        void loadPNG(const std::string& imagePath);

        bool strHasEnding(const std::string& string, const std::string& ending);

    protected:
        int width_, height_;
        unsigned char* data_;
};

#endif
