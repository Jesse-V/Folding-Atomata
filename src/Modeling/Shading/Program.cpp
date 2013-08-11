
#include "Program.hpp"
#include <sstream>
#include <iostream>


cs5400::Program::Program(
    const std::shared_ptr<VertexShader>& vertex,
    const std::shared_ptr<FragmentShader>& fragment):
    handle_(glCreateProgram()), vertex_(vertex), fragment_(fragment)
{}



cs5400::Program::~Program()
{
    glDeleteProgram(handle_);
}



GLuint cs5400::Program::getHandle()
{
    return handle_;
}



std::shared_ptr<cs5400::Program> cs5400::makeProgram(
    const std::shared_ptr<VertexShader>& vertex,
    const std::shared_ptr<FragmentShader>& fragment
)
{
    auto program = std::make_shared<Program>(vertex, fragment);
    auto programHandle = program->getHandle();

    glAttachShader(programHandle, vertex->getHandle());
    glAttachShader(programHandle, fragment->getHandle());
    glLinkProgram (programHandle);

    GLint link_ok = GL_FALSE;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &link_ok);

    int bufLength = 0;
    GLchar buf[8192];
    glGetProgramInfoLog(programHandle, sizeof(buf), &bufLength, buf);
    checkGlError();

    if (link_ok)
    {
        std::cout << "Attached and linked shaders.";
        if (bufLength > 0)
            std::cout << " Program Info Log: " << std::endl << std::endl << buf;
        std::cout << std::endl;
    }
    else
    {
        std::stringstream stream("");
        stream << "Could not link shader program \n" << buf;
        throw std::runtime_error(stream.str());
    }

    return program;
}



void checkGlError()
{
    std::cout << "GL status: ";

    auto error = glGetError();
    switch (error)
    {
        case GL_NO_ERROR :
            std::cout << "GL_NO_ERROR" << std::endl;
            break;

        default:
            std::cout << error << std::endl;

            std::stringstream stream("");
            stream << "GL ERROR: 0x" << std::hex << error;
            throw std::runtime_error(stream.str());
    }
}
