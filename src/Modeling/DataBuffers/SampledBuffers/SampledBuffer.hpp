
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

#ifndef MAPPED_BUFFER
#define MAPPED_BUFFER

#include "../OptionalDataBuffer.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <string>

/**
    A SampledBuffer maps an image onto a fragments of a polygon.
    An image is first generated CPU-side or loaded from a file. It's then
    saved into the GPU's VRAM, and is then applied to a set of fragments on the
    screen. This last step is done with UV coordinates: a coordinate system of
    (0, 0) to (1, 1) that specifies a point on the image. A sampler is used in
    the fragment shader to convert from camera-space to UV coordinates in 
    image-space. This results in each fragment knowing its corresponding
    location in the image, and the data there can the be used in ways
    defined by the classes which inherit this class. When this mapping causes
    the image to appear to be enlarged or reduced, linear minimap filters are
    applied for a smooth blending effect. 
**/
class SampledBuffer : public OptionalDataBuffer
{
    public:
        SampledBuffer(const std::string& imagePath,
                      const std::vector<GLfloat>& coordinateMap);
        ~SampledBuffer();
        //default copy constructor should be fine

        void loadBMP(const std::string& imagePath);
        void deleteBufferFromRAM();

        virtual void initialize(GLuint handle);
        virtual void store();

    protected:
        void storeImage();
        void storeCoordMap();

        bool strHasEnding(const std::string& string, const std::string& ending);

    protected:
        int imgWidth_, imgHeight_;
        unsigned char* data_;
        bool isValid_ = false;

        std::vector<GLfloat> coordinateMap_;
        GLuint vbo_coords_;
};

#endif
