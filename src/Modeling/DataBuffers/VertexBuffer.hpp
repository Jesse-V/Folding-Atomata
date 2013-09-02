
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

#ifndef VERTEX_BUFFER
#define VERTEX_BUFFER

/**
    A VertexBuffer contains a set of points in 3D space. These help define
    the shape of a Mesh. A VertexBuffer can be drawn by itself when using an
    IndexBuffer is not appropriate, but it is often a better idea to use
    an IndexBuffer in conjunction with a VertexBuffer to define a Mesh.
    A VertexBuffer has GLSL data GPU-side: each vertex is given a 3D vector that
    defines its position in model-space.
**/

#include "DataBuffer.hpp"
#include "glm/glm.hpp"
#include <vector>

class VertexBuffer : public DataBuffer
{
    public:
        VertexBuffer(const std::vector<glm::vec3>& vertices);
        virtual void initialize(GLuint programHandle);
        virtual void store();
        virtual void enable();
        virtual void disable();
        void draw(GLenum mode);

        virtual SnippetPtr getVertexShaderGLSL();
        virtual SnippetPtr getFragmentShaderGLSL();

        std::vector<glm::vec3> getVertices();

    private:
        void storePoints();
        void enableVertices();

    private:
        std::vector<glm::vec3> vertices_;
        GLuint vertexBuffer_;
        GLint vertexAttrib_;
};

#endif
