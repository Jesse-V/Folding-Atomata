
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

#include "VertexBuffer.hpp"


VertexBuffer::VertexBuffer(const std::vector<glm::vec3>& vertices):
    vertices_(vertices)
{}



// Store the vertices in a GPU buffer
void VertexBuffer::store(GLuint programHandle)
{
    glGenBuffers(1, &vertexBuffer_);
    vertexAttrib_ = glGetAttribLocation(programHandle, "vertex");

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(glm::vec3),
        vertices_.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexAttrib_);
}



void VertexBuffer::enable()
{
    glEnableVertexAttribArray(vertexAttrib_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    glVertexAttribPointer(vertexAttrib_, 3, GL_FLOAT, GL_FALSE, 0, 0);
}



void VertexBuffer::disable()
{
    glDisableVertexAttribArray(vertexAttrib_);
}



void VertexBuffer::draw(GLenum mode)
{
    glDrawElements(mode, (int)vertices_.size(), GL_UNSIGNED_INT, 0);
}



std::vector<glm::vec3> VertexBuffer::getVertices()
{
    return vertices_;
}



SnippetPtr VertexBuffer::getVertexShaderGLSL()
{
    return std::make_shared<ShaderSnippet>();
}



SnippetPtr VertexBuffer::getFragmentShaderGLSL()
{
    return std::make_shared<ShaderSnippet>();
}
