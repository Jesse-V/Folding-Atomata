
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

#ifndef TRAJECTORY
#define TRAJECTORY

#include "Topology.hpp"
#include "Snapshot.hpp"

class Trajectory
{
    public:
        Trajectory(const std::shared_ptr<Topology> topology);
        std::shared_ptr<Topology> getTopology();

        void addSnapshot(const SnapshotPtr& newSnapshot);
        SnapshotPtr getSnapshot(int index);
        int countSnapshots();
    
    private:
        std::shared_ptr<Topology> topology_;
        std::vector<SnapshotPtr> snapshots_;
};

typedef std::shared_ptr<Trajectory> TrajectoryPtr;

#endif

/*
    Trajectory
        has topology
            has list of atoms
                each has symbol, charge, radius, mass, and atomic number
            has list of bond
                each has pair of indexes, referring to connected atoms
        has list of snapshots
            each has list of xyz coordinates for each atom
*/
