
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

#ifndef DATA_BUFFER
#define DATA_BUFFER

/**
    A DataBuffer is a collection of OpenGL data for a Model that is attached
    to a Program. DataBuffers often have variables or arrays of information,
    which must be initialized/registered on the GPU and then stored in VRAM.
**/

#include "Modeling/Shading/ShaderUtilizer.hpp"
#include "Modeling/Shading/ShaderSnippet.hpp"
#include <GL/glew.h>
#include <GL/glut.h>

class DataBuffer : public ShaderUtilizer
{
    public:
        virtual void store(GLuint programHandle) = 0;
        virtual void enable() = 0;
        virtual void disable() = 0;
};

#endif
