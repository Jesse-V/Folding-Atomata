
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

#include "InstancedModel.hpp"
#include "Modeling/Shading/Program.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>


InstancedModel::InstancedModel(const std::shared_ptr<Mesh>& mesh) :
    mesh_(mesh), isVisible_(true)
{}



InstancedModel::InstancedModel(const std::shared_ptr<Mesh>& mesh,
               const glm::mat4& modelMatrix) :
    InstancedModel(mesh)
{
    modelMatrices_.push_back(modelMatrix);
}



InstancedModel::InstancedModel(const std::shared_ptr<Mesh>& mesh,
               const std::vector<glm::mat4>& modelMatrices) :
    InstancedModel(mesh)
{
    modelMatrices_ = modelMatrices;
}



InstancedModel::InstancedModel(const std::shared_ptr<Mesh>& mesh,
               const glm::mat4& modelMatrix,
               const BufferList& optionalDBs) :
    InstancedModel(mesh, modelMatrix)
{
    optionalDBs_ = optionalDBs;
}



InstancedModel::InstancedModel(const std::shared_ptr<Mesh>& mesh,
               const std::vector<glm::mat4>& modelMatrices,
               const BufferList& optionalDBs) :
    InstancedModel(mesh, modelMatrices)
{
    optionalDBs_ = optionalDBs;
}



void InstancedModel::saveAs(GLuint programHandle)
{
    std::cout << "Storing Model under Program " << programHandle << ": { ";

    mesh_->store(programHandle);

    /*std::cout << typeid(*mesh_).name() << " ";

    for (auto buffer : optionalDBs_)
    {
        buffer->store(programHandle);
        std::cout << typeid(*buffer).name() << " ";
    }
*/
    std::cout << "}" << std::endl;
    checkGlError();
}



// Objects that are not 'visible' will not be rendered
void InstancedModel::setVisible(bool visible)
{
    isVisible_ = visible;
}



BufferList InstancedModel::getOptionalDataBuffers()
{
    return optionalDBs_;
}



void InstancedModel::render(GLuint programHandle)
{
    static GLint matrixLoc = -1;
    if (matrixLoc < 0)
        matrixLoc = glGetUniformLocation(programHandle, "modelMatrix");

    if (isVisible_)
    {
        mesh_->enable();
        auto ib = mesh_->indexBuffer_;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->indexBuffer_);
        int size = (int)ib->indices_.size();

        for (glm::mat4 modelMatrix : modelMatrices_)
        {
            glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
            glDrawElements(GL_TRIANGLE_STRIP, size, GL_UNSIGNED_INT, 0);
        }
    }
}



void InstancedModel::enableDataBuffers()
{
    mesh_->enable();

    for (auto buffer : optionalDBs_)
        buffer->enable();
}



void InstancedModel::disableDataBuffers()
{
    mesh_->disable();

    for (auto buffer : optionalDBs_)
        buffer->disable();
}
