
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

#ifndef MODEL
#define MODEL

#include "Modeling/Mesh/Mesh.hpp"
#include "Modeling/DataBuffers/OptionalDataBuffer.hpp"
#include <vector>
#include <memory>

/**
    A Model is a polygon-based representation of a real-world object.
    Models are essentially defined by a Mesh, which specifies their shape
    as a set of small simply polygons (often triangles, sometimes quads).
    The Mesh is positioned and contoured in world space via a model matrix.
    Alone, this Mesh is completely colorless and is only visible by the Scene's
    ambient light. To include other properties, OptionalDataBuffers can be added
    to the Model. These can define per-vertex properties such as normals or colors,
    or even per-fragment properties such as textures or bump maps.
    All of these are saved under a cs5400::Program, a wrapper for the OpenGL
    program on the GPU. The setVisible method determines whether a Model and all
    of its corresponding data is rendered or not.
**/

typedef std::vector<std::shared_ptr<OptionalDataBuffer>> BufferList;

class Model
{
    public:
        Model(const std::shared_ptr<Mesh>& mesh);
        Model(const std::shared_ptr<Mesh>& mesh, const BufferList& optionalDBs);

        virtual void saveAs(GLuint programHandle);
        void setVisible(bool visible);
        void setModelMatrix(const glm::mat4& matrix); // model -> world coords
        void setRenderMode(GLenum newMode);
        void render(GLuint programHandle);

        BufferList getOptionalDataBuffers();

    private:
        void enableDataBuffers();
        void disableDataBuffers();

    protected:
        std::shared_ptr<Mesh> mesh_;
        BufferList optionalDBs_;
        glm::mat4 modelMatrix_;
        GLint matrixUniform_;
        bool isVisible_;
};

#endif
