
#include "TrajectoryParser.hpp"
#include <sstream>
#include <algorithm>
#include <string>
#include <iostream>


TrajectoryPtr TrajectoryParser::parse(const std::string& trajStr)
{
    std::cout << "Parsing trajectory PyON... ";

    auto top = parseTopology(getBetween(trajStr, "PyON 1 topology\n{\n", "\n}\n---"));
    TrajectoryPtr trajectory = std::make_shared<Trajectory>(top);

    std::size_t sstart = 0;
    while (sstart != std::string::npos)
    {
        auto snapshotStr = getBetween(trajStr, "PyON 1 positions\n[", "]\n---", sstart);
        trajectory->addSnapshot(parseSnapshot(snapshotStr));
        sstart = trajStr.find("PyON 1 positions", sstart + 1);
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

    std::size_t index = 0;
    while (index != std::string::npos)
    {
        auto positionStr = getBetween(snapshotStr, "[", "]", index);
        auto tokens = explodeAndTrim(positionStr, ',', " \n");

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
    auto tokens = explodeAndTrim(isolated, ',', " \"");

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
    auto tokens = explodeAndTrim(isolated, ',', " ");

    int atomIndexA, atomIndexB;
    std::istringstream(tokens[0]) >> atomIndexA;
    std::istringstream(tokens[1]) >> atomIndexB;

    return std::make_shared<Bond>(atomIndexA, atomIndexB);
}



/*
    Returns the contents of the string between the header and the footer,
    excluding both
*/
std::string TrajectoryParser::getBetween(const std::string& str,
                const std::string& header, const std::string& footer, std::size_t start
)
{
    std::size_t head = str.find(header, start);
    std::size_t foot = str.find(footer, head);
    return str.substr(head + header.length(), foot - head - header.length());
}



/******************** STRING PROCESSING HELPERS ***************************/



/*
    Explodes the string around the given delimiter, then trims away
    any of the given whitespace characters from sides of the tokens.
*/
std::vector<std::string> TrajectoryParser::explodeAndTrim(const std::string& str, 
                                    char delim, const std::string& whitespaces
)
{
    auto tokens = explode(str, delim);
    std::transform(tokens.begin(), tokens.end(), tokens.begin(), 
        [&](const std::string& token)
        {
            return trim(token, whitespaces);
        }
    );

    return tokens;
}



/*
    Explodes the string around the given delimiter. (Reproduction of PHP's explode)
*/
std::vector<std::string> TrajectoryParser::explode(const std::string& str, char delim)
{
    std::vector<std::string> tokens;
    std::stringstream stream(str);
    std::string item;
    while (std::getline(stream, item, delim))
        tokens.push_back(item);        

    return tokens;
}



/*
    Trims any of the given whitespace characters off of both sides of the given string.
*/
std::string TrajectoryParser::trim(const std::string& str, const std::string& whitespaces)
{
    std::size_t start = str.find_first_not_of(whitespaces);
    std::size_t end = str.find_last_not_of(whitespaces);

    if (start != std::string::npos && end != std::string::npos)
        return str.substr(start, end);
    else
        return "";
}
