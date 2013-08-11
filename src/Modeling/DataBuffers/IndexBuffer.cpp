
#include "IndexBuffer.hpp"
#include <algorithm>
#include <stdexcept>


IndexBuffer::IndexBuffer(std::size_t length, GLenum type) :
    acceptedAsType_(type)
{
    indices_.reserve(length);
    for (std::size_t j = 0; j < length; j++)
        indices_.push_back((GLuint)j);
}



IndexBuffer::IndexBuffer(const std::vector<GLuint>& indices, GLenum type) :
    indices_(indices), acceptedAsType_(type)
{}



void IndexBuffer::initialize(GLuint)
{
    glGenBuffers(1, &indexBuffer_);
}



void IndexBuffer::store()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint),
        indices_.data(), GL_STATIC_DRAW);
}



void IndexBuffer::draw(GLenum mode)
{
    glDrawElements(mode, (int)indices_.size(), GL_UNSIGNED_INT, 0);
}



bool IndexBuffer::canInterpretAs(GLenum type)
{
    return (type == GL_TRIANGLES && indices_.size() % 3 == 0) ||
           (type == GL_QUADS     && indices_.size() % 4 == 0);
}



std::vector<Triangle> IndexBuffer::reinterpretAsTriangles()
{
    if (!canInterpretAs(GL_TRIANGLES))
        throw std::runtime_error("Cannot reinterpret indices as Triangles!");

    std::vector<Triangle> triangles;
    triangles.reserve(indices_.size() / 3);

    for (std::size_t j = 0; j < indices_.size(); j += 3)
    {
        Triangle triangle(indices_[j], indices_[j + 1], indices_[j + 2]);
        triangles.push_back(triangle);
    }

    return triangles;
}



std::vector<Quad> IndexBuffer::reinterpretAsQuads()
{
    if (!canInterpretAs(GL_QUADS))
        throw std::runtime_error("Cannot reinterpret indices as Quads!");

    std::vector<Quad> quads;
    quads.reserve(indices_.size() / 4);

    for (std::size_t j = 0; j < indices_.size(); j += 4)
    {
        Quad quad(indices_[j], indices_[j + 1], indices_[j + 2], indices_[j + 3]);
        quads.push_back(quad);
    }

    return quads;
}



std::vector<Triangle> IndexBuffer::castToTriangles()
{
    if (acceptedAsType_ == GL_TRIANGLES)
        return reinterpretAsTriangles();

    if (acceptedAsType_ != GL_QUADS)
        throw std::runtime_error("Unknown face type for IndexBuffer!");

    if (!canInterpretAs(GL_QUADS))
        throw std::runtime_error("Cannot convert indices from Quads!");

    //convert from Quads to Triangles
    std::vector<Triangle> triangles;
    for (std::size_t j = 0; j < indices_.size(); j += 4)
    {
        Triangle a(indices_[j + 0], indices_[j + 1], indices_[j + 3]);
        Triangle b(indices_[j + 1], indices_[j + 2], indices_[j + 3]);

        triangles.push_back(a);
        triangles.push_back(b);
    }
    return triangles;
}



void IndexBuffer::enable()
{}



void IndexBuffer::disable()
{}



SnippetPtr IndexBuffer::getVertexShaderGLSL()
{
    return std::make_shared<ShaderSnippet>();
}



SnippetPtr IndexBuffer::getFragmentShaderGLSL()
{
    return std::make_shared<ShaderSnippet>();
}
