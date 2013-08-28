
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

#include "Mesh.hpp"
#include <algorithm>
#include <stdexcept>


Mesh::Mesh(const std::shared_ptr<VertexBuffer>& vertexBuffer, GLenum mode) :
    vertexBuffer_(vertexBuffer), indexBuffer_(nullptr), renderingMode_(mode)
{}



Mesh::Mesh(const std::shared_ptr<VertexBuffer>& vertexBuffer,
           const std::shared_ptr<IndexBuffer>& indexBuffer,
           GLenum mode) :
    vertexBuffer_(vertexBuffer), indexBuffer_(indexBuffer), renderingMode_(mode)
{}



void Mesh::initialize(GLuint programHandle)
{
    vertexBuffer_->initialize(programHandle);
    if (indexBuffer_)
        indexBuffer_->initialize(programHandle);
}



void Mesh::store()
{
    vertexBuffer_->store();
    if (indexBuffer_)
        indexBuffer_->store();
}



void Mesh::enable()
{
    vertexBuffer_->enable();
    if (indexBuffer_)
        indexBuffer_->enable();
}



void Mesh::disable()
{
    vertexBuffer_->disable();
    if (indexBuffer_)
        indexBuffer_->disable();
}



void Mesh::draw()
{
    //indexBuffer is drawn with priority iff it is available
    if (indexBuffer_)
        indexBuffer_->draw(renderingMode_);
    else
        vertexBuffer_->draw(renderingMode_);
}



std::vector<glm::vec3> Mesh::getVertices()
{
    return vertexBuffer_->getVertices();
}



std::vector<Triangle> Mesh::getTriangles()
{
    if (!indexBuffer_)
    {
        return std::make_shared<IndexBuffer>(
            vertexBuffer_->getVertices().size(),
            GL_TRIANGLES
        )->castToTriangles();
    }

    return indexBuffer_->castToTriangles();
}



std::shared_ptr<VertexBuffer> Mesh::getVertexBuffer()
{
    return vertexBuffer_;
}



std::shared_ptr<IndexBuffer> Mesh::getIndexBuffer()
{
    return indexBuffer_;
}



SnippetPtr Mesh::getVertexShaderGLSL()
{
    return std::make_shared<ShaderSnippet>();
}



SnippetPtr Mesh::getFragmentShaderGLSL()
{
    return std::make_shared<ShaderSnippet>();
}
