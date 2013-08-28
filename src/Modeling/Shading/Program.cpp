
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
