
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

#include "TextureBuffer.hpp"


TextureBuffer::TextureBuffer(
    const std::string& imagePath,
    const std::vector<GLfloat>& coordinateMap):
    SampledBuffer(imagePath, coordinateMap)
{}



void TextureBuffer::initialize(GLuint programHandle)
{
    SampledBuffer::initialize(programHandle);
    texCoordAttrib_ = glGetAttribLocation(programHandle, "textureCoordinate");
}



void TextureBuffer::store()
{
    SampledBuffer::store();
    glVertexAttribPointer(texCoordAttrib_, 2, GL_FLOAT, GL_FALSE, 0, 0);
}



void TextureBuffer::enable()
{
    glEnableVertexAttribArray(texCoordAttrib_);
}



void TextureBuffer::disable()
{
    glDisableVertexAttribArray(texCoordAttrib_);
}



//todo: some of this code belongs to SampledBuffer, base class's method could be called
SnippetPtr TextureBuffer::getVertexShaderGLSL()
{
    return std::make_shared<ShaderSnippet>(
        R".(
            //TextureBuffer fields
            attribute vec2 textureCoordinate;
            varying vec2 UVcoordinate;
        ).",
        R".(
            //TextureBuffer methods
        ).",
        R".(
            //TextureBuffer main method code
            UVcoordinate = textureCoordinate;
        )."
    );
}



SnippetPtr TextureBuffer::getFragmentShaderGLSL()
{
    return std::make_shared<ShaderSnippet>(
        R".(
            //TextureBuffer fields
            uniform sampler2D textureSampler;
            varying vec2 UVcoordinate;
        ).",
        R".(
            //TextureBuffer methods
        ).",
        R".(
            //TextureBuffer main method code
            colors.material = texture2D(textureSampler, UVcoordinate).rgb;
        )."
    );
}
