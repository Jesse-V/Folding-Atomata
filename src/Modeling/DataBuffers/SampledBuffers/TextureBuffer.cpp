
#include "TextureBuffer.hpp"


TextureBuffer::TextureBuffer(
    const std::string& imagePath,
    const std::vector<GLfloat>& coordinateMap):
    SampledBuffer(imagePath, coordinateMap)
{}



void TextureBuffer::initialize(GLuint programHandle)
{
    SampledBuffer::initialize(programHandle);
    texCoordAttrib_ = glGetAttribLocation(programHandle, "textureCoordinate");
}



void TextureBuffer::store()
{
    SampledBuffer::store();
    glVertexAttribPointer(texCoordAttrib_, 2, GL_FLOAT, GL_FALSE, 0, 0);
}



void TextureBuffer::enable()
{
    glEnableVertexAttribArray(texCoordAttrib_);
}



void TextureBuffer::disable()
{
    glDisableVertexAttribArray(texCoordAttrib_);
}



//todo: some of this code belongs to SampledBuffer, base class's method could be called
SnippetPtr TextureBuffer::getVertexShaderGLSL()
{
    return std::make_shared<ShaderSnippet>(
        R".(
            //TextureBuffer fields
            in vec2 textureCoordinate;
            out vec2 UVcoordinate;
        ).",
        R".(
            //TextureBuffer methods
        ).",
        R".(
            //TextureBuffer main method code
            UVcoordinate = textureCoordinate;
        )."
    );
}



SnippetPtr TextureBuffer::getFragmentShaderGLSL()
{
    return std::make_shared<ShaderSnippet>(
        R".(
            //TextureBuffer fields
            uniform sampler2D textureSampler;
            in vec2 UVcoordinate;
        ).",
        R".(
            //TextureBuffer methods
        ).",
        R".(
            //TextureBuffer main method code
            colors.textureColor = texture(textureSampler, UVcoordinate).rgb;
        )."
    );
}
