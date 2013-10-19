
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

#include "TexturedCube.hpp"
#include <iostream>
#include "Modeling/Shading/Program.hpp"
#include <string.h>


TexturedCube::TexturedCube(
    const std::shared_ptr<Image>& positiveX,
    const std::shared_ptr<Image>& negativeX,
    const std::shared_ptr<Image>& positiveY,
    const std::shared_ptr<Image>& negativeY,
    const std::shared_ptr<Image>& positiveZ,
    const std::shared_ptr<Image>& negativeZ
) :
    positiveX_(positiveX), negativeX_(negativeX),
    positiveY_(positiveY), negativeY_(negativeY),
    positiveZ_(positiveZ), negativeZ_(negativeZ)
{}


//glDeleteTextures(1, &texture_id);



void TexturedCube::store(GLuint programHandle)
{
    /*
        http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
        https://www.opengl.org/wiki/Cubemap_Texture
        http://stackoverflow.com/questions/14505969/the-best-way-to-texture-a-cube-in-opengl
        https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_06
    */

    glGenTextures(1, &cubeTexture_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture_);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glActiveTexture(GL_TEXTURE0);

    mapTo(GL_TEXTURE_CUBE_MAP_POSITIVE_X, positiveX_);
    mapTo(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, negativeX_);
    mapTo(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, positiveY_);
    mapTo(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, negativeY_);
    mapTo(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, positiveZ_);
    mapTo(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, negativeZ_);

    textureCoordinates_ = glGetAttribLocation(programHandle, "UV");
    if (textureCoordinates_ == -1)
        throw std::runtime_error("Could not bind UV attribute");
    //glEnableVertexAttribArray(textureCoordinates_);
}



void TexturedCube::enable()
{
    glVertexAttribPointer(textureCoordinates_, 3, GL_FLOAT, GL_FALSE, 0, 0);
}



void TexturedCube::disable()
{
    //glDisableVertexAttribArray(texCoordAttrib_);
}



void TexturedCube::mapTo(GLenum target, const std::shared_ptr<Image>& img)
{
    glTexImage2D(target, 0, GL_RGB, img->getWidth(), img->getWidth(), 0,
        GL_BGR, GL_UNSIGNED_BYTE, img->getImageData()); //image must be square
}



//todo: some of this code belongs to SampledBuffer, base class's method could be called
SnippetPtr TexturedCube::getVertexShaderGLSL()
{
    return std::make_shared<ShaderSnippet>(
        R".(
            //TexturedCube fields
            attribute vec3 UV;
            varying vec3 UVf;
        ).",
        R".(
            //TexturedCube methods
        ).",
        R".(
            //TexturedCube main method code
            UVf = UV;
        )."
    );
}



SnippetPtr TexturedCube::getFragmentShaderGLSL()
{
    return std::make_shared<ShaderSnippet>(
        R".(
            //TexturedCube fields
            uniform samplerCube cubeSampler;
            varying vec3 UVf;
        ).",
        R".(
            //TexturedCube methods
        ).",
        R".(
            //TexturedCube main method code
            colors.material = textureCube(cubeSampler, UVf).rgb;
        )."
    );
}
