
#include "VertexBuffer.hpp"
#include <algorithm>


VertexBuffer::VertexBuffer(const std::vector<glm::vec3>& vertices):
    vertices_(vertices)
{}



void VertexBuffer::initialize(GLuint programHandle)
{
    glGenBuffers(1, &vertexBuffer_);
    vertexAttrib_ = glGetAttribLocation(programHandle, "vertex");
}



// Store the vertices in a GPU buffer
void VertexBuffer::store()
{
    std::vector<GLfloat> rawPoints;
    for_each (vertices_.begin(), vertices_.end(),
        [&](const glm::vec3& vert)
        {
            rawPoints.push_back(vert.x);
            rawPoints.push_back(vert.y);
            rawPoints.push_back(vert.z);
        });

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    glBufferData(GL_ARRAY_BUFFER, rawPoints.size() * sizeof(GLfloat),
        rawPoints.data(), GL_STATIC_DRAW);
}



void VertexBuffer::enable()
{
    glEnableVertexAttribArray(vertexAttrib_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    glVertexAttribPointer(vertexAttrib_, 3, GL_FLOAT, GL_FALSE, 0, 0);
}



void VertexBuffer::disable()
{
    glDisableVertexAttribArray(vertexAttrib_);
}



void VertexBuffer::draw(GLenum mode)
{
    glDrawElements(mode, (int)vertices_.size(), GL_UNSIGNED_INT, 0);
}



std::vector<glm::vec3> VertexBuffer::getVertices()
{
    return vertices_;
}



SnippetPtr VertexBuffer::getVertexShaderGLSL()
{
    return std::make_shared<ShaderSnippet>();
}



SnippetPtr VertexBuffer::getFragmentShaderGLSL()
{
    return std::make_shared<ShaderSnippet>();
}
