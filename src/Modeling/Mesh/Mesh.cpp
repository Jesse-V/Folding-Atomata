
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
