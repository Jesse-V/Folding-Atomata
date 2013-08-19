
#include "ColorBuffer.hpp"
#include <algorithm>
#include <iostream>


ColorBuffer::ColorBuffer(const glm::vec3& color, std::size_t count)
{
    colors_.reserve(count);
    for (int j = 0; j < count; j++)
        colors_.push_back(color);
}



ColorBuffer::ColorBuffer(const std::vector<glm::vec3>& colors) :
    colors_(colors)
{}



std::vector<glm::vec3> ColorBuffer::getColors()
{
    return colors_;
}



void ColorBuffer::initialize(GLuint programHandle)
{
    glGenBuffers(1, &colorBuffer_);
    colorAttrib_ = glGetAttribLocation(programHandle, "vertexColor");
    std::cout << colorBuffer_ << ", " << colorAttrib_ << std::endl;
}



// Store the normals in a GPU buffer
void ColorBuffer::store()
{
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer_);
    glBufferData(GL_ARRAY_BUFFER, colors_.size() * sizeof(glm::vec3),
        colors_.data(), GL_STATIC_DRAW);
}



void ColorBuffer::enable()
{
    glEnableVertexAttribArray(colorAttrib_);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer_);
    glVertexAttribPointer(colorAttrib_, 3, GL_FLOAT, GL_FALSE, 0, 0);
}



void ColorBuffer::disable()
{
    glDisableVertexAttribArray(colorAttrib_);
}



SnippetPtr ColorBuffer::getVertexShaderGLSL()
{
    return std::make_shared<ShaderSnippet>(
        R".(
            //ColorBuffer fields
            in vec3 vertexColor;
            out vec3 vertexColorBlend;
        ).",
        R".(
            //ColorBuffer methods
        ).",
        R".(
            //ColorBuffer main method code
            vertexColorBlend = vertexColor;
        )."
    );
}



SnippetPtr ColorBuffer::getFragmentShaderGLSL()
{
    return std::make_shared<ShaderSnippet>(
        R".(
            //ColorBuffer fields
            in vec3 vertexColorBlend;
        ).",
        R".(
            //ColorBuffer methods
        ).",
        R".(
            //ColorBuffer main method code
            colors.material = vertexColorBlend;
        )."
    );
}
