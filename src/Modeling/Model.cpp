
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

#include "Model.hpp"
#include "Modeling/Shading/Program.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <algorithm>
#include <iostream>


Model::Model(const std::shared_ptr<Mesh>& mesh) :
    mesh_(mesh), modelMatrix_(glm::mat4()), isVisible_(true)
{}



Model::Model(const std::shared_ptr<Mesh>& mesh, const BufferList& optionalDBs) :
    Model(mesh)
{
    optionalDBs_ = optionalDBs;
    
    /*
    std::cout << "Created a Model with { ";
    for_each (optionalDBs_.begin(), optionalDBs_.end(),
        [&](const std::shared_ptr<OptionalDataBuffer>& buffer)
        {
            std::cout << typeid(*buffer).name() << " ";
        }
    );

    std::cout << "} OptionalDataBuffers." << std::endl;
    */
}



void Model::saveAs(GLuint programHandle)
{
    std::cout << "Storing Model under Program " << programHandle << ": { ";

    mesh_->initialize(programHandle);
    mesh_->store();

    std::cout << typeid(*mesh_).name() << " ";

    for_each (optionalDBs_.begin(), optionalDBs_.end(),
        [&](const std::shared_ptr<OptionalDataBuffer>& buffer)
        {
            buffer->initialize(programHandle);
            buffer->store();

            std::cout << typeid(*buffer).name() << " ";
        });

    std::cout << "}" << std::endl;
    checkGlError();
}



// Objects that are not 'visible' will not be rendered
void Model::setVisible(bool visible)
{
    isVisible_ = visible;
}



// Set the matrix to convert from model coords to world coords
void Model::setModelMatrix(const glm::mat4& matrix)
{
    modelMatrix_ = matrix;
}



BufferList Model::getOptionalDataBuffers()
{
    return optionalDBs_;
}



void Model::render(GLuint programHandle)
{
    if (matrixUniform_ == 0)
        matrixUniform_ = glGetUniformLocation(programHandle, "modelMatrix");

    if (isVisible_)
    {
        glUniformMatrix4fv(matrixUniform_, 1, GL_FALSE, 
            glm::value_ptr(modelMatrix_)
        ); //necessary when multiple Models share a Program

        enableDataBuffers();
        mesh_->draw();
        disableDataBuffers();
    }
}



void Model::enableDataBuffers()
{
    mesh_->enable();

    for_each (optionalDBs_.begin(), optionalDBs_.end(),
        [&](const std::shared_ptr<OptionalDataBuffer>& buffer)
        {
            buffer->enable();
        });
}



void Model::disableDataBuffers()
{
    mesh_->disable();

    for_each (optionalDBs_.begin(), optionalDBs_.end(),
        [&](const std::shared_ptr<OptionalDataBuffer>& buffer)
        {
            buffer->disable();
        });
}
