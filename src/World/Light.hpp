
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

#ifndef LIGHT
#define LIGHT

#include "Modeling/Shading/ShaderUtilizer.hpp"
#include "glm/glm.hpp"
#include <GL/glew.h>

/**
    Defines a point-light, that is, a light defined from a point in space.
    A point-light can be simply defined as it is here: a position, color,
    and power. The power variable defines its intensity and how far it appears
    to spread out before it dissipates into negletable levels.
**/
class Light : public ShaderUtilizer
{
    public:
        Light(
            const glm::vec3& position = glm::vec3(0.0, 0.0, 0.0),
            const glm::vec3& color = glm::vec3(1, 1, 1),
            float power = 0.5f
        );

        void setPosition(const glm::vec3& newPos);
        void setColor(const glm::vec3& newColor);
        void setPower(float power);
        void setEmitting(bool emitting);

        glm::vec3 getPosition() const;
        glm::vec3 getColor() const;
        float getPower() const;
        bool isEmitting() const;

        void sync(GLuint handle, std::size_t lightID);

        virtual SnippetPtr getVertexShaderGLSL();
        virtual SnippetPtr getFragmentShaderGLSL();

    private:
        glm::vec3 position_, color_;
        float power_;
        bool emitting_;

        static std::size_t nLights_;
};

typedef std::vector<std::shared_ptr<Light>> LightList;

#endif
