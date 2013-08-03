
#include "TrajectoryParser.hpp"
#include <sstream>
#include <algorithm>
#include <string>
#include <iostream>


TrajectoryPtr TrajectoryParser::parse(const std::string& trajStr)
{
    auto top = parseTopology(getBetween(trajStr, "PyON 1 topology\n{\n", "\n}\n---"));
    auto trajectory = std::make_shared<Trajectory>(top);

    //add snapshots here

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



std::vector<AtomPtr> TrajectoryParser::parseAtoms(const std::string& topStr)
{
    std::vector<AtomPtr> atoms;
    auto str = getBetween(topStr, "\"atoms\": [\n", "]\n  ],\n");
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
    auto isolated = getBetween(atomStr, "[", "]"); //removes brackets
    auto tokens = explode(isolated, ','); //explodes along commas
    std::transform(tokens.begin(), tokens.end(), tokens.begin(), 
        [&](const std::string& token)
        {
            return trim(token, " \""); //remove spaces and quotes
        }
    );

    int atomicNumber;
    float charge, radius, mass;

    std::istringstream(tokens[1]) >> atomicNumber;
    std::istringstream(tokens[2]) >> charge;
    std::istringstream(tokens[3]) >> radius;
    std::istringstream(tokens[4]) >> mass;

    return std::make_shared<Atom>(tokens[0], atomicNumber, charge, radius, mass);
}



std::vector<BondPtr> TrajectoryParser::parseBonds(const std::string& topStr)
{
    auto bondStr = getBetween(topStr, "\"bonds\": [\n", "]\n");
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
    auto isolated = getBetween(bondStr, "[", "]");
    auto tokens = explode(isolated, ','); //explodes along commas
    std::transform(tokens.begin(), tokens.end(), tokens.begin(), 
        [&](const std::string& token)
        {
            return trim(token, " "); //remove spaces
        }
    );

    int atomIndexA, atomIndexB;
    std::istringstream(tokens[0]) >> atomIndexA;
    std::istringstream(tokens[1]) >> atomIndexB;

    return std::make_shared<Bond>(atomIndexA, atomIndexB);
}



std::string TrajectoryParser::getBetween(const std::string& str,
                const std::string& header, const std::string& footer, int start
)
{
    int head = str.find(header, start);
    int foot = str.find(footer, head);
    return str.substr(head + header.length(), foot - head - header.length());
}



std::vector<std::string> TrajectoryParser::explode(const std::string& str, char delim)
{
    std::vector<std::string> tokens;
    std::stringstream stream(str);
    std::string item;
    while (std::getline(stream, item, delim))
        tokens.push_back(item);        

    return tokens;
}



std::string TrajectoryParser::trim(const std::string& str, const std::string& whitespaces)
{
    std::size_t start = str.find_first_not_of(whitespaces);
    std::size_t end = str.find_last_not_of(whitespaces);

    if (start != std::string::npos && end != std::string::npos)
        return str.substr(start, end);
    else
        return "";
}
