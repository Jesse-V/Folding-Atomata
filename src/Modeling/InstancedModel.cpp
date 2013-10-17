
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
    mesh_(mesh), cachedHandle_(0), isVisible_(true)
{}



InstancedModel::InstancedModel(const std::shared_ptr<Mesh>& mesh,
                               const glm::mat4& modelMatrix) :
    InstancedModel(mesh)
{
    modelMatrices_.push_back(modelMatrix);
}



InstancedModel::InstancedModel(const std::shared_ptr<Mesh>& mesh,
                               const BufferList& optionalDBs) :
    InstancedModel(mesh)
{
    optionalDBs_ = optionalDBs;
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
    InstancedModel(mesh)
{
    modelMatrices_ = modelMatrices;
    optionalDBs_ = optionalDBs;
}



void InstancedModel::saveAs(GLuint programHandle)
{
    std::cout << "Storing Model under Program " << programHandle << ": { ";

    mesh_->store(programHandle);

    std::cout << typeid(*mesh_).name() << " ";

    for (auto buffer : optionalDBs_)
    {
        buffer->store(programHandle);
        std::cout << typeid(*buffer).name() << " ";
    }

    std::cout << "}" << std::endl;
    checkGlError();
}


void InstancedModel::addInstance(const glm::mat4& instanceModelMatrix)
{
    modelMatrices_.push_back(instanceModelMatrix);
}



void InstancedModel::render(GLuint programHandle)
{
    if (cachedHandle_ != programHandle) //bit of adaptable caching
    {
        cachedHandle_ = programHandle;
        matrixModelLocation_ = glGetUniformLocation(programHandle, "modelMatrix");
    }

    if (isVisible_)
    {
        enableDataBuffers();

        for (glm::mat4 modelMatrix : modelMatrices_)
        {
            glUniformMatrix4fv(matrixModelLocation_, 1, GL_FALSE,
                glm::value_ptr(modelMatrix));
            mesh_->draw();
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



// Objects that are not 'visible' will not be rendered
void InstancedModel::setVisible(bool visible)
{
    isVisible_ = visible;
}



BufferList InstancedModel::getOptionalDataBuffers()
{
    return optionalDBs_;
}
