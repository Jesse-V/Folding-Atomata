
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
    positiveZ_(positiveZ), negativeZ_(negativeZ),
    tex_(0)
{}



void CubeTextureMap::initialize(GLuint programHandle)
{
    glActiveTexture(GL_TEXTURE1);
    glGenBuffers(1, &tex_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex_);

    texMapLocation_ = glGetUniformLocation(programHandle, "texMap");
    glUniform1i(texMapLocation_, 1); //corresponds to unit 1
}



void CubeTextureMap::store()
{
    mapToFace(GL_TEXTURE_CUBE_MAP_POSITIVE_X, positiveX_);
    mapToFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, negativeX_);
    mapToFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, positiveY_);
    mapToFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, negativeY_);
    mapToFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, positiveZ_);
    mapToFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, negativeZ_);

    //when MAGnifying the image (no bigger mipmap available),
    //use LINEAR filtering
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //when MINifying the image, use a LINEAR blend of two mipmaps,
    //each filtered LINEARLY too
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    //generate mipmaps
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}



void CubeTextureMap::enable()
{
    //glEnableVertexAttribArray(texCoordAttrib_);
}



void CubeTextureMap::disable()
{
    //glDisableVertexAttribArray(texCoordAttrib_);
}



void CubeTextureMap::mapToFace(GLenum target, const std::shared_ptr<Image>& img)
{
    glTexImage2D(target, 0, GL_RGB, img->getWidth(), img->getHeight(),
        0, GL_BGR, GL_UNSIGNED_BYTE, img->getImageData());
}



//todo: some of this code belongs to SampledBuffer, base class's method could be called
SnippetPtr CubeTextureMap::getVertexShaderGLSL()
{
    return std::make_shared<ShaderSnippet>(
        R".(
            //CubeTextureMap fields
            varying vec3 R;
        ).",
        R".(
            //CubeTextureMap methods
        ).",
        R".(
            //CubeTextureMap main method code
            R = vec3(0.5);
        )."
    );
}



SnippetPtr CubeTextureMap::getFragmentShaderGLSL()
{
    return std::make_shared<ShaderSnippet>(
        R".(
            //CubeTextureMap fields
            uniform samplerCube texMap;
        ).",
        R".(
            //CubeTextureMap methods
        ).",
        R".(
            //CubeTextureMap main method code
            colors.material = textureCube(texMap, UVcoordinate).rgb;
        )."
    );
}
