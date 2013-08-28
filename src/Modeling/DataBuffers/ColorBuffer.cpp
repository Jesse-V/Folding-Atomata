
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

#include "ColorBuffer.hpp"
#include <algorithm>


ColorBuffer::ColorBuffer(const glm::vec3& color, std::size_t count)
{
    colors_.reserve(count);
    for (std::size_t j = 0; j < count; j++)
        colors_.push_back(color);
}



ColorBuffer::ColorBuffer(const std::vector<glm::vec3>& colors) :
    colors_(colors)
{}



std::vector<glm::vec3> ColorBuffer::getColors()
{
    return colors_;
}



void ColorBuffer::initialize(GLuint programHandle)
{
    glGenBuffers(1, &colorBuffer_);
    colorAttrib_ = glGetAttribLocation(programHandle, "vertexColor");
}



// Store the normals in a GPU buffer
void ColorBuffer::store()
{
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer_);
    glBufferData(GL_ARRAY_BUFFER, colors_.size() * sizeof(glm::vec3),
        colors_.data(), GL_STATIC_DRAW);
}



void ColorBuffer::enable()
{
    glEnableVertexAttribArray(colorAttrib_);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer_);
    glVertexAttribPointer(colorAttrib_, 3, GL_FLOAT, GL_FALSE, 0, 0);
}



void ColorBuffer::disable()
{
    glDisableVertexAttribArray(colorAttrib_);
}



SnippetPtr ColorBuffer::getVertexShaderGLSL()
{
    return std::make_shared<ShaderSnippet>(
        R".(
            //ColorBuffer fields
            attribute vec3 vertexColor;
            varying vec3 vertexColorBlend;
        ).",
        R".(
            //ColorBuffer methods
        ).",
        R".(
            //ColorBuffer main method code
            vertexColorBlend = vertexColor;
        )."
    );
}



SnippetPtr ColorBuffer::getFragmentShaderGLSL()
{
    return std::make_shared<ShaderSnippet>(
        R".(
            //ColorBuffer fields
            varying vec3 vertexColorBlend;
        ).",
        R".(
            //ColorBuffer methods
        ).",
        R".(
            //ColorBuffer main method code
            colors.material = vertexColorBlend;
        )."
    );
}
