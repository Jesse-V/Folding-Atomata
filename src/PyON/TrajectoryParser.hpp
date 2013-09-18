
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

#ifndef TRAJECTORY_PARSER
#define TRAJECTORY_PARSER

/**
    Creates a Trajectory class by analyzing PyON-formatted strings returned
    from FAHClient. The Trajectory class is a container for other container
    classes that all have a very structured has-a relationship. This class
    primarily takes the strings from the FAHClient API and returns
    a Trajectory class.
**/

#include "Trajectory/Trajectory.hpp"
#include "PyON/StringManip.hpp"

class TrajectoryParser
{
    public:
        TrajectoryParser(const std::string& pyon, bool removeSpaces = true);
        TrajectoryPtr parse(bool filter = true);

    private:
        TopologyPtr parseTopology();
        std::vector<AtomPtr> parseAtoms(const Indexes& topologySubStr);
        AtomPtr parseAtom(const Indexes& atomSpan);
        BondList parseBonds(const Indexes& topologySpan,
                            const std::vector<AtomPtr>& atoms
        );
        Indexes parseBond(const Indexes& bondline);
        void parsePositions(const TrajectoryPtr& trajectory);
        PositionMap parseSnapshot(const Indexes& snapshotRange,
                                  const TopologyPtr& topology
        );

    private:
         std::string pyon_;
};

#endif
