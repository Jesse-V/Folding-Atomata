
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

#include "Trajectory.hpp"
#include <cfloat>


Trajectory::Trajectory(const std::shared_ptr<Topology> topology) :
    topology_(topology)
{}



std::shared_ptr<Topology> Trajectory::getTopology()
{
    return topology_;
}



BoundingBoxPtr Trajectory::calculateBoundingBox()
{
    float smallestX = FLT_MAX, largestX = FLT_MIN;
    float smallestY = FLT_MAX, largestY = FLT_MIN;
    float smallestZ = FLT_MAX, largestZ = FLT_MIN;

    std::size_t nAtoms = getTopology()->getAtoms().size();
    for (auto snapshot : snapshots_)
    {
        for (std::size_t j = 0; j < nAtoms; j++)
        {
            glm::vec3 position = snapshot->getPosition(j);

            if (position.x < smallestX)
                smallestX = position.x;
            if (position.x > largestX)
                largestX = position.x;
            if (position.y < smallestY)
                smallestY = position.y;
            if (position.y > largestY)
                largestY = position.y;
            if (position.z < smallestZ)
                smallestZ = position.z;
            if (position.z > largestZ)
                largestZ = position.z;
        }
    }

    return std::make_shared<BoundingBox>(
                glm::vec3(smallestX, smallestY, smallestZ),
                glm::vec3(largestX,  largestY,  largestZ));
}



void Trajectory::addSnapshot(const SnapshotPtr& newSnapshot)
{
    snapshots_.push_back(newSnapshot);
}



SnapshotPtr Trajectory::getSnapshot(int index)
{
    return snapshots_[(std::size_t)index];
}



int Trajectory::countSnapshots()
{
    return (int)snapshots_.size();
}
