
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

#include "Image.hpp"
#include <png++/image.hpp>
#include <png++/rgb_pixel.hpp>
#include <stdexcept>
#include <iostream>


Image::Image(const std::string& imagePath)
{
    std::cout << "Loading image from " << imagePath << " ...";
    
    if (strHasEnding(imagePath, ".png"))
        loadPNG(imagePath);
    else if (strHasEnding(imagePath, ".bmp"))
        loadBMP(imagePath);
    else
        throw std::runtime_error("Unrecognized file extension for " + imagePath);

    std::cout << " done." << std::endl;
}



void Image::loadBMP(const std::string& imagePath)
{
    unsigned char header[54];

    FILE * file = fopen(imagePath.c_str(), "rb"); //read binary .bmp file
    if (!file)
        throw std::runtime_error("Unable to open image at " + imagePath);

    if(fread(header, 1, 54, file) != 54)
        throw std::runtime_error(imagePath + " is not a valid .bmp image");

    if (header[0] != 'B' || header[1] != 'M')
        throw std::runtime_error(imagePath + " is not a valid .bmp image");

    auto char12Ptr = &header[0x12], char16Ptr = &header[0x16];
    width_  = *(int*)char12Ptr;
    height_ = *(int*)char16Ptr;

    auto char22Ptr = &header[0x22];
    int imageSize  = *(int*)char22Ptr;
    if (imageSize == 0)
        imageSize = width_ * height_ * 3;

    data_ = new unsigned char[imageSize];

    // Read the actual data from the file into the buffer
    auto readSize = fread(data_, 1, (unsigned long)imageSize, file);
    if (readSize != (unsigned long)imageSize)
        throw std::runtime_error("Invalid fread of " + imagePath);

    //Everything is in memory now, the file can be closed
    fclose(file);
}



void Image::loadPNG(const std::string& imagePath)
{
    png::image<png::rgb_pixel> image(imagePath);

    auto pixbuf = image.get_pixbuf();

    width_  = (int)image.get_width();
    height_ = (int)image.get_height();

    int imageSize = width_ * height_ * 3;

    data_ = new unsigned char[imageSize];

    for (int i = 0; i < imageSize; i += 3)
    {
        int x = (i / 3) % width_;
        int y = (i / 3) / width_;
        auto pix = pixbuf.get_pixel((std::size_t)x, (std::size_t)y);

        data_[i + 0] = pix.blue;
        data_[i + 1] = pix.green;
        data_[i + 2] = pix.red;
    }
}



bool Image::strHasEnding(const std::string& string, const std::string& ending)
{
    if (string.length() >= ending.length())
    {
        auto value = string.compare(string.length() - ending.length(),
                                    ending.length(), ending);
        return 0 == value;
    }
    else
        return false;
}



int Image::getWidth()
{
    return width_;
}



int Image::getHeight()
{
    return height_;
}



unsigned char* Image::getImageData()
{
    return data_;
}
