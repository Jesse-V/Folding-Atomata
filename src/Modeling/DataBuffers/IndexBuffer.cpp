
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

#include "IndexBuffer.hpp"
#include <algorithm>
#include <stdexcept>


IndexBuffer::IndexBuffer(std::size_t length, GLenum type) :
    acceptedAsType_(type)
{
    indices_.reserve(length);
    for (std::size_t j = 0; j < length; j++)
        indices_.push_back((GLuint)j);
}



IndexBuffer::IndexBuffer(const std::vector<GLuint>& indices, GLenum type) :
    indices_(indices), acceptedAsType_(type)
{}



void IndexBuffer::store(GLuint)
{
    glGenBuffers(1, &indexBuffer_);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint),
        indices_.data(), GL_STATIC_DRAW);
}



void IndexBuffer::draw(GLenum mode)
{
    glDrawElements(mode, (int)indices_.size(), GL_UNSIGNED_INT, 0);
}



bool IndexBuffer::canInterpretAs(GLenum type)
{
    return (type == GL_TRIANGLES && indices_.size() % 3 == 0) ||
           (type == GL_QUADS     && indices_.size() % 4 == 0);
}



std::vector<Triangle> IndexBuffer::reinterpretAsTriangles()
{
    if (!canInterpretAs(GL_TRIANGLES))
        throw std::runtime_error("Cannot reinterpret indices as Triangles!");

    std::vector<Triangle> triangles;
    triangles.reserve(indices_.size() / 3);

    for (std::size_t j = 0; j < indices_.size(); j += 3)
    {
        Triangle triangle(indices_[j], indices_[j + 1], indices_[j + 2]);
        triangles.push_back(triangle);
    }

    return triangles;
}



std::vector<Quad> IndexBuffer::reinterpretAsQuads()
{
    if (!canInterpretAs(GL_QUADS))
        throw std::runtime_error("Cannot reinterpret indices as Quads!");

    std::vector<Quad> quads;
    quads.reserve(indices_.size() / 4);

    for (std::size_t j = 0; j < indices_.size(); j += 4)
    {
        Quad quad(indices_[j], indices_[j + 1], indices_[j + 2], indices_[j + 3]);
        quads.push_back(quad);
    }

    return quads;
}



std::vector<Triangle> IndexBuffer::castToTriangles()
{
    if (acceptedAsType_ == GL_TRIANGLES)
        return reinterpretAsTriangles();

    if (acceptedAsType_ != GL_QUADS)
        throw std::runtime_error("Unknown face type for IndexBuffer!");

    if (!canInterpretAs(GL_QUADS))
        throw std::runtime_error("Cannot convert indices from Quads!");

    //convert from Quads to Triangles
    std::vector<Triangle> triangles;
    for (std::size_t j = 0; j < indices_.size(); j += 4)
    {
        Triangle a(indices_[j + 0], indices_[j + 1], indices_[j + 3]);
        Triangle b(indices_[j + 1], indices_[j + 2], indices_[j + 3]);

        triangles.push_back(a);
        triangles.push_back(b);
    }
    return triangles;
}



void IndexBuffer::enable()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
}



void IndexBuffer::disable()
{}



SnippetPtr IndexBuffer::getVertexShaderGLSL()
{
    return std::make_shared<ShaderSnippet>();
}



SnippetPtr IndexBuffer::getFragmentShaderGLSL()
{
    return std::make_shared<ShaderSnippet>();
}
