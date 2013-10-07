
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

#ifndef MESH
#define MESH

/**
    A Mesh contains the set of simple polygons that comprise a representation
    of a real-world object. A Mesh must contain a VertexBuffer, which defines
    the set of points in space the make up the Mesh. A Mesh can optionally have
    an IndexBuffer, which construct a set of simple polygons from these points.
    For example, if GL_TRIANGLES is the rendering mode passed to Mesh::draw,
    a Mesh could define a square by just a VertexBuffer of
    {(0, 0), (1, 1), (0, 1), (0, 0), (1, 0), (1, 1)} which would tell OpenGL
    to render the two triangles defined by the three sets of points.
    Alternatively, a Mesh initialized with both a VertexBuffer and an IndexBuffer
    would be {(0, 0), (1, 1), (0, 1), (1, 0)} and {(0, 1, 2), (0, 3, 1)}.
    The memory benefits get more significant with more complex models where
    a vertex is used to help define more than one face. A Mesh can be
    rendered as a cloud of points or a wireframe (depending on the mode passed
    to Mesh::draw) but is most often drawn with GL_TRIANGLES.
    Note that "glEnable(GL_CULL_FACE); glCullFace(GL_BACK);" prevents OpenGL
    from drawing triangles that are not facing the viewer, a significant
    rendering optimization.
**/

#include "Modeling/DataBuffers/IndexBuffer.hpp"
#include "Modeling/DataBuffers/VertexBuffer.hpp"
#include "Triangle.struct"

class Mesh : public DataBuffer
{
    public:
        Mesh(const std::shared_ptr<VertexBuffer>& vertexBuffer,
             GLenum mode = GL_TRIANGLES
        );
        Mesh(const std::shared_ptr<VertexBuffer>& vertexBuffer,
             const std::shared_ptr<IndexBuffer>& indexBuffer,
             GLenum mode = GL_TRIANGLES
        );

        virtual void store(GLuint programHandle);
        virtual void enable();
        virtual void disable();
        virtual void draw();

        virtual SnippetPtr getVertexShaderGLSL();
        virtual SnippetPtr getFragmentShaderGLSL();

        std::vector<glm::vec3> getVertices();
        std::vector<Triangle> getTriangles();

        std::shared_ptr<VertexBuffer> getVertexBuffer();
        std::shared_ptr<IndexBuffer> getIndexBuffer();

    private:
        std::shared_ptr<VertexBuffer> vertexBuffer_;
        std::shared_ptr<IndexBuffer> indexBuffer_;
        GLenum renderingMode_;
};

#endif
