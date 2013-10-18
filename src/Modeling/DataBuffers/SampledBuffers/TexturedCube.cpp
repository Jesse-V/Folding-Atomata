
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

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, // target
               0,  // level, 0 = base, no minimap,
               GL_RGB, // internalformat
               positiveX_->getWidth(),  // width
               positiveX_->getHeight(),  // height
               0,  // border, always 0 in OpenGL ES
               GL_RGB,  // format
               GL_UNSIGNED_BYTE, // type
               positiveX_->getImageData());

    attribute_texcoord = glGetAttribLocation(programHandle, "texcoord");
    if (attribute_texcoord == -1)
        throw std::runtime_error("Could not bind texcoord attribute");

    GLfloat cube_texcoords[] = {
        // front
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0,
    };
    glGenBuffers(1, &vbo_cube_texcoords);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texcoords), cube_texcoords, GL_STATIC_DRAW);

    /*std::cout << "In initialize1:" << std::endl;
    std::cout.flush();
    checkGlError();

    //glActiveTexture(GL_TEXTURE1);
    glGenBuffers(1, &tex_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex_);



    //texMapLocation_ = glGetUniformLocation(programHandle, "texMap");
    //glUniform1i(texMapLocation_, 1); //corresponds to unit 1

    std::cout << "In initialize2:" << std::endl;
    std::cout.flush();
    checkGlError();


    std::cout << "In store0:" << std::endl;
    std::cout.flush();
    checkGlError();

    mapToFace(GL_TEXTURE_CUBE_MAP_POSITIVE_X, positiveX_);
    mapToFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, negativeX_);
    mapToFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, positiveY_);
    mapToFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, negativeY_);
    mapToFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, positiveZ_);
    mapToFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, negativeZ_);

    std::cout << "In store1:" << std::endl;
    std::cout.flush();
    checkGlError();

    //when MAGnifying the image (no bigger mipmap available),
    //use LINEAR filtering
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //when MINifying the image, use a LINEAR blend of two mipmaps,
    //each filtered LINEARLY too
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    //generate mipmaps
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    std::cout << "In store2:" << std::endl;
    std::cout.flush();
    checkGlError();*/
}



void TexturedCube::enable()
{
    //glEnableVertexAttribArray(texCoordAttrib_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glUniform1i(uniform_mytexture, /*GL_TEXTURE*/0);

    glEnableVertexAttribArray(attribute_texcoord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
    glVertexAttribPointer(
        attribute_texcoord, // attribute
        2,                  // number of elements per vertex, here (x,y)
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        0,                  // no extra data between each position
        0                   // offset of first element
    );
}



void TexturedCube::disable()
{
    //glDisableVertexAttribArray(texCoordAttrib_);
}



void TexturedCube::mapToFace(GLenum target, const std::shared_ptr<Image>& img)
{
    glTexImage2D(target, 0, GL_RGB, img->getWidth(), img->getHeight(),
        0, GL_RGB, GL_UNSIGNED_BYTE, img->getImageData());
}



//todo: some of this code belongs to SampledBuffer, base class's method could be called
SnippetPtr TexturedCube::getVertexShaderGLSL()
{
    return std::make_shared<ShaderSnippet>(
        R".(
            //TexturedCube fields
            attribute vec2 texcoord;
            varying vec2 f_texcoord;
        ).",
        R".(
            //TexturedCube methods
        ).",
        R".(
            //TexturedCube main method code
            f_texcoord = texcoord;
        )."
    );
}



SnippetPtr TexturedCube::getFragmentShaderGLSL()
{
    return std::make_shared<ShaderSnippet>(
        R".(
            //TexturedCube fields
            uniform sampler2D mytexture;
            varying vec2 f_texcoord;
        ).",
        R".(
            //TexturedCube methods
        ).",
        R".(
            //TexturedCube main method code
            colors.material = texture2D(mytexture, f_texcoord).rgb;
        )."
    );
}
