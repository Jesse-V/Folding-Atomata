
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

#ifndef INDEX_BUFFER
#define INDEX_BUFFER

/**
    An IndexBuffer defines the faces of a Mesh by referencing n-tuples of points
    in space that describe each face. This helps efficiently define a Mesh when
    vertexes are shared by multiple faces, so it's generally recommended to use
    an IndexBuffer in conjunction with a VertexBuffer whenever possible.
    An IndexBuffer has no GLSL data; it is just used by OpenGL.
**/

#include "DataBuffer.hpp"
#include "Modeling/Mesh/Triangle.struct"
#include "Modeling/Mesh/Quad.struct"
#include <vector>

class IndexBuffer : public DataBuffer
{
    public:
        IndexBuffer(std::size_t length, GLenum type = GL_TRIANGLES);
        IndexBuffer(const std::vector<GLuint>& indices, GLenum type = GL_TRIANGLES);

        void draw(GLenum mode);
        bool canInterpretAs(GLenum type);
        std::vector<Triangle> reinterpretAsTriangles();
        std::vector<Quad> reinterpretAsQuads();
        std::vector<Triangle> castToTriangles();

        virtual void store(GLuint programHandle);
        virtual void enable();
        virtual void disable();

        virtual SnippetPtr getVertexShaderGLSL();
        virtual SnippetPtr getFragmentShaderGLSL();

    private:
        std::vector<GLuint> indices_;
        GLuint indexBuffer_;
        GLenum acceptedAsType_;
};

#endif
