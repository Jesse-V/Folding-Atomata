
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
#include <sstream>
#include <iostream>


TrajectoryPtr TrajectoryParser::parse(const std::string& trajStr)
{
    std::cout << "Parsing trajectory PyON... ";

    const std::string topBegin = "PyON 1 topology\n{\n", topEnd = "\n}\n---";
    auto top = parseTopology(StringManip::between(trajStr, topBegin, topEnd));
    TrajectoryPtr trajectory = std::make_shared<Trajectory>(top);

    std::size_t index = 0;
    const std::string snapBegin = "PyON 1 positions\n[", snapEnd = "]\n---";
    while (index != std::string::npos)
    {
        auto snapStr = StringManip::between(trajStr, snapBegin, snapEnd, index);
        trajectory->addSnapshot(parseSnapshot(snapStr));
        index = trajStr.find("PyON 1 positions", index + 1);
    }

    std::cout << "done." << std::endl;
    
    return trajectory;
}



/* Given:
  "atoms": [
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
TopologyPtr TrajectoryParser::parseTopology(const std::string& topStr)
{
    auto atoms = parseAtoms(topStr);
    auto bonds = parseBonds(topStr);

    return std::make_shared<Topology>(atoms, bonds);
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
SnapshotPtr TrajectoryParser::parseSnapshot(const std::string& snapshotStr)
{
    SnapshotPtr snapshot = std::make_shared<Snapshot>();

    std::size_t index = snapshotStr.find("[");
    while (index != std::string::npos)
    {
        auto positionStr = StringManip::between(snapshotStr, "[", "]", index);
        auto tokens = StringManip::explodeAndTrim(positionStr, ',', " \n");

        float x, y, z;
        std::istringstream(tokens[0]) >> x;
        std::istringstream(tokens[1]) >> y;
        std::istringstream(tokens[2]) >> z;

        snapshot->addPosition(glm::vec3(x, y, z));
        index = snapshotStr.find("[", index + 1);
    }

    return snapshot;
}



std::vector<AtomPtr> TrajectoryParser::parseAtoms(const std::string& topStr)
{
    std::vector<AtomPtr> atoms;
    auto str = StringManip::between(topStr, "\"atoms\": [\n", "]\n  ],\n");
    std::stringstream atomsStream(str);
    std::string line;
    while (std::getline(atomsStream, line))
        atoms.push_back(parseAtom(line));

    return atoms;
}



/* Given:
    ["N", -0.96, 1.7063, 14.007, 7],
*/
AtomPtr TrajectoryParser::parseAtom(const std::string& atomStr)
{
    auto isolated = StringManip::between(atomStr, "[", "]"); //removes brackets
    auto tokens = StringManip::explodeAndTrim(isolated, ',', " \"");

    int number;
    float charge, radius, mass;

    std::istringstream(tokens[1]) >> number;
    std::istringstream(tokens[2]) >> charge;
    std::istringstream(tokens[3]) >> radius;
    std::istringstream(tokens[4]) >> mass;

    return std::make_shared<Atom>(tokens[0], number, charge, radius, mass);
}



std::vector<BondPtr> TrajectoryParser::parseBonds(const std::string& topStr)
{
    auto bondStr = StringManip::between(topStr, "\"bonds\": [\n", "]\n");
    std::stringstream bondsStream(bondStr);
    
    std::vector<BondPtr> bonds;
    std::string line;
    while (std::getline(bondsStream, line))
        bonds.push_back(parseBond(line));

    return bonds;
}



/* Given:
    [2356, 2358],
*/
BondPtr TrajectoryParser::parseBond(const std::string& bondStr)
{
    auto isolated = StringManip::between(bondStr, "[", "]");
    auto tokens = StringManip::explodeAndTrim(isolated, ',', " ");

    int atomIndexA, atomIndexB;
    std::istringstream(tokens[0]) >> atomIndexA;
    std::istringstream(tokens[1]) >> atomIndexB;

    return std::make_shared<Bond>(atomIndexA, atomIndexB);
}
