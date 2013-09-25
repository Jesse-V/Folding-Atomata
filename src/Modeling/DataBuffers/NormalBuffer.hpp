
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

#ifndef NORMAL_BUFFER
#define NORMAL_BUFFER

/**
    A NormalBuffer is an OptionalDataBuffer that can be added to a Model to
    give each vertex a normal vector. Ideally, the normal should point outward
    from the bounds of the polygon defined by that point, but this is often
    difficult to calculate, especially for very complex models where a vertex
    is shared between multiple faces. Therefore, three different
    normal calculation algorithms are included. The NormalBuffer is constructed
    with the results of one of these algorithms, and a per-vertex variable
    is defined in GLSL. These can be blended GPUs-side for per-fragment normals.
**/

#include "OptionalDataBuffer.hpp"
#include "Modeling/Mesh/Mesh.hpp"
#include "Modeling/Mesh/Triangle.struct"
#include <vector>
#include <string>

class NormalBuffer : public OptionalDataBuffer
{
    public:
        NormalBuffer(const std::vector<glm::vec3>& normals);
        virtual void store(GLuint programHandle);
        virtual void enable();
        virtual void disable();

        virtual SnippetPtr getVertexShaderGLSL();
        virtual SnippetPtr getFragmentShaderGLSL();

        static std::vector<glm::vec3> calcNormalsMWA(
            const std::shared_ptr<Mesh>& mesh
        );
        static std::vector<glm::vec3> calcNormalsMWASER(
            const std::shared_ptr<Mesh>& mesh
        );
        static std::vector<glm::vec3> calcNormalsMWE(
            const std::shared_ptr<Mesh>& mesh
        );

    private:
        static std::vector<glm::vec3> calculateTriangleNormals(
            const std::shared_ptr<Mesh>& mesh,
            std::vector<std::vector<size_t>>& vtmap
        );

    private:
        std::vector<glm::vec3> normals_;

        GLuint normalBuffer_;
        GLint normalAttrib_;
};

#endif
