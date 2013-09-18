
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

#include "TrajectoryParser.hpp"
#include "StringManip.hpp"
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <iostream>


TrajectoryParser::TrajectoryParser(const std::string& pyon, bool removeSpaces) :
    pyon_(pyon)
{
    if (removeSpaces)
    {
        char space[] = " ";
        pyon_.erase(std::remove(pyon_.begin(), pyon_.end(), space[0]), pyon_.end());
    }
}



TrajectoryPtr TrajectoryParser::parse(bool filter)
{
    std::cout << "Parsing trajectory PyON... " << std::endl;
    std::cout.flush();
    auto top = parseTopology();
    TrajectoryPtr trajectory = std::make_shared<Trajectory>(top);

    parsePositions(trajectory);

    std::cout << "... done parsing trajectory." << std::endl;

    return trajectory;
}



/* Given:
"atoms":[
["N", -0.96, 1.7063, 14.007, 7],
["O3G", -0.9, 1.52, 15.9994, 8],
["MG", 2, 1, 24.305, 12],
["MG", 2, 1, 24.305, 12]
],
"bonds": [
[4273, 4276],
[4273, 4275],
[2181, 2182]
]
*/
TopologyPtr TrajectoryParser::parseTopology()
{
    const std::string BEGIN = "PyON1topology\n{\n", END = "\n}\n---";
    const auto FULL = std::make_pair(0, pyon_.size());
    const auto TOPOLOGY_SPAN = StringManip::between(pyon_, FULL, BEGIN, END);

    auto atoms = parseAtoms(TOPOLOGY_SPAN);
    auto bonds = parseBonds(TOPOLOGY_SPAN, atoms);

    return std::make_shared<Topology>(atoms, bonds);
}



std::vector<AtomPtr> TrajectoryParser::parseAtoms(const Indexes& topologySpan)
{
    const std::string BEGIN = "\"atoms\":[\n", END = "]\n],\n";
    const auto SPAN = StringManip::between(pyon_, topologySpan, BEGIN, END);

    std::vector<AtomPtr> atoms;
    std::size_t lineStart = SPAN.first;
    while (lineStart < SPAN.second)
    {
        std::size_t lineEnd = pyon_.find('\n', lineStart);
        atoms.push_back(parseAtom(std::make_pair(lineStart, lineEnd)));
        lineStart = lineEnd + 1;
    }

    return atoms;
}



/* Given:
["N", -0.96, 1.7063, 14.007, 7],
*/
AtomPtr TrajectoryParser::parseAtom(const Indexes& atomSpan)
{
    auto range = StringManip::between(pyon_, atomSpan, "[", "]");
    auto isolated = pyon_.substr(range.first, range.second - range.first);
    auto tokens = StringManip::explodeAndTrim(isolated, ',', " \"");

    int number;
    float charge, radius, mass;

    std::istringstream(tokens[1]) >> number;
    std::istringstream(tokens[2]) >> charge;
    std::istringstream(tokens[3]) >> radius;
    std::istringstream(tokens[4]) >> mass;

    return std::make_shared<Atom>(tokens[0], number, charge, radius, mass);
}



BondList TrajectoryParser::parseBonds(const Indexes& topologySpan,
                                      const std::vector<AtomPtr>& atoms
)
{
    const auto BEGIN = "\"bonds\":[\n", END = "]\n";
    auto bondDataRange = StringManip::between(pyon_, topologySpan, BEGIN, END);

    BondList bonds;
    std::size_t lineStart = bondDataRange.first;
    while (lineStart < bondDataRange.second)
    {
        std::size_t lineEnd = pyon_.find("\n", lineStart);
        auto bond = parseBond(std::make_pair(lineStart, lineEnd));
        bonds.push_back(std::make_pair(atoms[bond.first], atoms[bond.second]));
        lineStart = lineEnd + 1;
    }

    return bonds;
}



/* Given:
[2356, 2358],
*/
Indexes TrajectoryParser::parseBond(const Indexes& bondline)
{
    auto range = StringManip::between(pyon_, bondline, "[", "]");
    auto isolated = pyon_.substr(range.first, range.second - range.first);
    auto tokens = StringManip::explodeAndTrim(isolated, ',', " ");

    std::size_t atomIndexA, atomIndexB;
    std::istringstream(tokens[0]) >> atomIndexA;
    std::istringstream(tokens[1]) >> atomIndexB;

    return std::make_pair(atomIndexA, atomIndexB);
}



void TrajectoryParser::parsePositions(const TrajectoryPtr& trajectory)
{
    std::cout << "Parsing all snapshots... " << std::endl;
    std::cout.flush();

    const std::string BEGIN = "PyON1positions\n[", END = "]\n---";
    std::size_t index = pyon_.find(BEGIN, 0);
    while (index != std::string::npos)
    {
        auto range = std::make_pair(index, pyon_.size());
        auto snapshotRange = StringManip::between(pyon_, range, BEGIN, END);
        auto snap = parseSnapshot(snapshotRange, trajectory->getTopology());
        trajectory->addSnapshot(snap);
        index = pyon_.find(BEGIN, index + 1);
    }

    std::cout << "... done parsing snapshots." << std::endl;
    std::cout.flush();
}


/* Given:
[
-15.150061,
26.855776,
10.119355
],
[
-15.447186,
26.083978,
10.699146
],
[
-14.151439,
26.92564,
10.253361
]
*/
PositionMap TrajectoryParser::parseSnapshot(const Indexes& snapshotRange,
                                            const TopologyPtr& topology
)
{
    PositionMap snapshot;
std::cout << "A" << std::endl;
std::cout.flush();
    std::size_t index = pyon_.find("[", snapshotRange.first);
    std::size_t count = 0;
    while (index <= snapshotRange.second)
    {
        auto range = std::make_pair(index, snapshotRange.second);
        auto pos = StringManip::between(pyon_, range, "[", "]");
        auto positionStr = pyon_.substr(pos.first, pos.second - pos.first + 1);
        auto tokens = StringManip::explodeAndTrim(positionStr, ',', " \n");
        //std::cout << positionStr << std::endl;

        //float x, y, z;
        float x = ::atof(tokens[0].c_str());
        float y = ::atof(tokens[1].c_str());
        float z = ::atof(tokens[2].c_str());
        //std::istringstream(tokens[0]) >> x;
        //std::istringstream(tokens[1]) >> y;
        //std::istringstream(tokens[2]) >> z;

        snapshot[topology->getAtoms()[count]] = glm::vec3(x, y, z);
        index = pyon_.find("[", index + 1);
        count++;
    }
std::cout << "B" << std::endl;
std::cout.flush();
    return snapshot;
}


