
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

#ifndef CS5400_PROGRAM
#define CS5400_PROGRAM

#include <memory>
#include "Shader.hpp"

namespace cs5400
{
    class Program
    {
        public:
            Program(
                const std::shared_ptr<VertexShader>& vertex,
                const std::shared_ptr<FragmentShader>& fragment);
            ~Program();
            GLuint getHandle();

        private:
            GLuint handle_;
            std::shared_ptr<VertexShader> vertex_;
            std::shared_ptr<FragmentShader> fragment_;
    };

    std::shared_ptr<Program> makeProgram(
        const std::shared_ptr<VertexShader>& vertex,
        const std::shared_ptr<FragmentShader>& fragment);
}

void checkGlError();

typedef std::shared_ptr<cs5400::Program> ProgramPtr;

#endif
