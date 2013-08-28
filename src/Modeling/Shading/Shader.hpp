
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

#ifndef CS5400_SHADER
#define CS5400_SHADER

#include <GL/glew.h>
#include <GL/glut.h>
#include <fstream>
#include <string>
#include <stdexcept>
#include <memory>

namespace cs5400
{
    namespace detail
    {
        struct VertexShaderTag{};
        struct FragmentShaderTag{};

        template<typename T> struct ShaderType;

        template<>
        struct ShaderType<VertexShaderTag>
        {
            static const GLenum value = GL_VERTEX_SHADER;
        };

        template<>
        struct ShaderType<FragmentShaderTag>
        {
            static const GLenum value = GL_FRAGMENT_SHADER;
        };
    }


    template<typename T>
    class Shader
    {
        public:
            Shader():
                handle(glCreateShader(detail::ShaderType<T>::value))
            {}

            ~Shader()
            {
                glDeleteShader(handle);
            }

            GLuint getHandle()
            {
                return handle;
            }

        private:
            GLuint handle;
    };

    typedef Shader<detail::VertexShaderTag> VertexShader;
    typedef Shader<detail::FragmentShaderTag> FragmentShader;

    std::shared_ptr<VertexShader> makeVertexShader(const std::string& path);
    std::shared_ptr<VertexShader> makeVertexShaderStr(const std::string& shaderCode);

    std::shared_ptr<FragmentShader> makeFragmentShader(const std::string& path);
    std::shared_ptr<FragmentShader> makeFragmentShaderStr(const std::string& shaderCode);
}

#endif
