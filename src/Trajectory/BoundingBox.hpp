
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

#ifndef BOUNDING_BOX
#define BOUNDING_BOX

#include "glm/glm.hpp"
#include <memory>

class BoundingBox
{
    public:
        BoundingBox(const glm::vec3& mimimum, const glm::vec3& maximum);
        bool intersectsWith(const std::shared_ptr<BoundingBox>& otherBox);
        BoundingBox& operator+=(const glm::vec3& offset);
        glm::vec3 getMinimum();
        glm::vec3 getMaximum();

    private:
        glm::vec3 minimum_, maximum_;
};

typedef std::shared_ptr<BoundingBox> BoundingBoxPtr;

#endif
