
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

#ifndef SHADER_UTILIZER
#define SHADER_UTILIZER

#include "ShaderSnippet.hpp"
#include <memory>
#include <vector>

typedef std::shared_ptr<ShaderSnippet> SnippetPtr;
typedef std::vector<SnippetPtr> SnippetList;

/**
    A ShaderUntilizer is a purely abstract class that declares two methods
    for fetching GLSL code. Many classes in /Modeling as well as in /World
    use OpenGL calls to hook into GLSL variables, so they require corresponding
    GLSL code to declare and handle those variables accordingly GPU-side.
    Although more modern versions of OpenGL/GLSL use geometry shaders,
    the ShaderUtilizer declares methods for returning only GLSL for 
    vertex and fragment shaders. Subclasses define the implementation of what
    code is returned.
**/
class ShaderUtilizer
{
    public:
        virtual SnippetPtr getVertexShaderGLSL() = 0;
        virtual SnippetPtr getFragmentShaderGLSL() = 0;
};

#endif
