
#include "SampledBuffer.hpp"
#include <png++/image.hpp>
#include <png++/rgb_pixel.hpp>
#include <stdexcept>
#include <iostream>


SampledBuffer::SampledBuffer(
    const std::string& imagePath,
    const std::vector<GLfloat>& coordinateMap):
    coordinateMap_(coordinateMap)
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



SampledBuffer::~SampledBuffer()
{
    deleteBufferFromRAM();
}



void SampledBuffer::initialize(GLuint programHandle)
{
    glGenBuffers(1, &vbo_coords_);
}



void SampledBuffer::store()
{
    storeImage();
    storeCoordMap();
}



//ideas: http://stackoverflow.com/questions/4983532/what-are-the-texture-coordinates-for-a-cube-in-opengl
void SampledBuffer::storeImage() //TODO: THE ISSUES WITH #22 MAY BE HERE
{
    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth_, imgHeight_, 0, GL_BGR, GL_UNSIGNED_BYTE, data_);

    // When MAGnifying the image (no bigger mipmap available),
    // use LINEAR filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // When MINifying the image, use a LINEAR blend of two mipmaps,
    // each filtered LINEARLY too
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Generate mipmaps, by the way.
    glGenerateMipmap(GL_TEXTURE_2D);

    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   85:  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
}



void SampledBuffer::storeCoordMap()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coords_);
    glBufferData(GL_ARRAY_BUFFER, coordinateMap_.size() * sizeof(GLfloat),
                 coordinateMap_.data(), GL_STATIC_DRAW);
}



void SampledBuffer::loadBMP(const std::string& imagePath)
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
    imgWidth_  = *(int*)char12Ptr;
    imgHeight_ = *(int*)char16Ptr;

    auto char22Ptr = &header[0x22];
    int imageSize  = *(int*)char22Ptr;
    if (imageSize == 0)
        imageSize = imgWidth_ * imgHeight_ * 3;

    data_ = new unsigned char[imageSize];

    // Read the actual data from the file into the buffer
    auto readSize = fread(data_, 1, (unsigned long)imageSize, file);
    if (readSize != (unsigned long)imageSize)
        throw std::runtime_error("Invalid fread of " + imagePath);

    //Everything is in memory now, the file can be closed
    fclose(file);

    isValid_ = true;
}



void SampledBuffer::loadPNG(const std::string& imagePath)
{
    png::image<png::rgb_pixel> image(imagePath);

    auto pixbuf = image.get_pixbuf();

    imgWidth_  = (int)image.get_width();
    imgHeight_ = (int)image.get_height();

    int imageSize = imgWidth_ * imgHeight_ * 3;

    data_ = new unsigned char[imageSize];

    for (int i = 0; i < imageSize; i += 3)
    {
        int x = (i / 3) % imgWidth_;
        int y = (i / 3) / imgWidth_;
        auto pix = pixbuf.get_pixel((std::size_t)x, (std::size_t)y);

        data_[i + 0] = pix.blue;
        data_[i + 1] = pix.green;
        data_[i + 2] = pix.red;
    }

    isValid_ = true;
}



void SampledBuffer::deleteBufferFromRAM()
{
    if (isValid_)
    {
        delete [] data_;
        isValid_ = false;
    }
}



bool SampledBuffer::strHasEnding(const std::string& string, const std::string& ending)
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
