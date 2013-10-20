
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

#include "BoundingBox.hpp"


BoundingBox::BoundingBox(const glm::vec3& minimum, const glm::vec3& maximum) :
    minimum_(minimum), maximum_(maximum)
{}



bool BoundingBox::intersectsWith(const std::shared_ptr<BoundingBox>& other)
{
    if (minimum_.x > other->maximum_.x || other->minimum_.x > maximum_.x)
        return false;
    if (minimum_.y > other->maximum_.y || other->minimum_.y > maximum_.y)
        return false;
    return minimum_.z <= other->maximum_.z && other->minimum_.z <= maximum_.z;
}



BoundingBox& BoundingBox::operator+=(const glm::vec3& offset)
{
    minimum_ += offset;
    maximum_ += offset;
    return *this;
}



glm::vec3 BoundingBox::getSizes()
{
    return maximum_ - minimum_;
}



glm::vec3 BoundingBox::getMinimum()
{
    return minimum_;
}



glm::vec3 BoundingBox::getMaximum()
{
    return maximum_;
}
