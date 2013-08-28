
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

#ifndef SHADER_SNIPPET
#define SHADER_SNIPPET

#include <string>

/**
    GLSL is a C-based language, and follows a very similar format. Fields follow
    preprocessor directives, methods follow fields, and main() follows fields.
    The ShaderSnippet class is a container for these three sections of code.
    The ShaderManager then assembles then in the correct order.
**/
class ShaderSnippet
{
    public:
        ShaderSnippet();
        ShaderSnippet(const std::string& fields,
            const std::string& methods, const std::string& mainBodyCode
        );

        std::string getFields();
        std::string getMethods();
        std::string getMainBodyCode();

    private:
        std::string fields_, methods_, mainBodyCode_;
};

#endif
