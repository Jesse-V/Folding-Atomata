
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

/**
    A Trajectory holds all the atoms, their atomic properties, and where they
    all are for each available snapshot. The Topology class is primarily
    responsible for holding all of the atomic and bond information, whereas
    this class provides access to a Topology instance as well as access to
    a list of PositionMaps. PositionMaps are a mapping (using a hashtable) of
    atoms to their positions. This allows for direct and efficient lookup
    of an atom's position without needing to know the atom's index, which can
    be useful for certain algorithms such as the ones in ProteinAnalysis.cpp.
**/

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
