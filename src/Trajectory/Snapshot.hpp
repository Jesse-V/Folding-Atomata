
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

#ifndef SNAPSHOT
#define SNAPSHOT

#include "glm/glm.hpp"
#include <vector>
#include <memory>

class Snapshot
{
    public:
        void addPosition(const glm::vec3& position);
        glm::vec3 getPosition(std::size_t atomIndex);

    private:
        std::vector<glm::vec3> positions_;
};

typedef std::shared_ptr<Snapshot> SnapshotPtr;

#endif
