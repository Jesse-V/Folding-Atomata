
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
