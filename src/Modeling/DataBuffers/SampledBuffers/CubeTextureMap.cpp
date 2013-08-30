
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

#include "CubeTextureMap.hpp"


CubeTextureMap::CubeTextureMap(
    const std::shared_ptr<Image> positiveX,
    const std::shared_ptr<Image> negativeX,
    const std::shared_ptr<Image> positiveY,
    const std::shared_ptr<Image> negativeY,
    const std::shared_ptr<Image> positiveZ,
    const std::shared_ptr<Image> negativeZ
) :
    positiveX_(positiveX), negativeX_(negativeX),
    positiveY_(positiveY), negativeY_(negativeY),
    positiveZ_(positiveZ), negativeZ_(negativeZ)
{}



void CubeTextureMap::initialize(GLuint programHandle)
{
    
    //texCoordAttrib_ = glGetAttribLocation(programHandle, "textureCoordinate");
}



void CubeTextureMap::store()
{
    
    //glVertexAttribPointer(texCoordAttrib_, 2, GL_FLOAT, GL_FALSE, 0, 0);
}



void CubeTextureMap::enable()
{
    //glEnableVertexAttribArray(texCoordAttrib_);
}



void CubeTextureMap::disable()
{
    //glDisableVertexAttribArray(texCoordAttrib_);
}



//todo: some of this code belongs to SampledBuffer, base class's method could be called
SnippetPtr CubeTextureMap::getVertexShaderGLSL()
{
    return std::make_shared<ShaderSnippet>(
        R".(
            //CubeTextureMap fields
            attribute vec2 textureCoordinate;
            varying vec2 UVcoordinate;
        ).",
        R".(
            //CubeTextureMap methods
        ).",
        R".(
            //CubeTextureMap main method code
            UVcoordinate = textureCoordinate;
        )."
    );
}



SnippetPtr CubeTextureMap::getFragmentShaderGLSL()
{
    return std::make_shared<ShaderSnippet>(
        R".(
            //CubeTextureMap fields
            uniform sampler2D textureSampler;
            varying vec2 UVcoordinate;
        ).",
        R".(
            //CubeTextureMap methods
        ).",
        R".(
            //CubeTextureMap main method code
            colors.material = texture2D(textureSampler, UVcoordinate).rgb;
        )."
    );
}
