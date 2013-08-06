
#ifndef TRAJECTORY_PARSER_HPP
#define TRAJECTORY_PARSER_HPP

#include "../Trajectory/Trajectory.hpp"

class TrajectoryParser
{
    public:
        TrajectoryPtr parse(const std::string& trajectoryStr);
        SnapshotPtr parseSnapshot(const std::string& snapshotStr);

    private:
        TopologyPtr parseTopology(const std::string& topologyStr);
        std::vector<AtomPtr> parseAtoms(const std::string& topologyStr);
        AtomPtr parseAtom(const std::string& atomStr);
        std::vector<BondPtr> parseBonds(const std::string& topologyStr);
        BondPtr parseBond(const std::string& bondStr);
        glm::vec3 parsePosition(const std::string& positionStr);

        //string processing helpers:
        std::string getBetween(const std::string& str, 
                               const std::string& header,
                               const std::string& footer, std::size_t start = 0
        );
        std::vector<std::string> explodeAndTrim(const std::string& str, 
                                    char delim, const std::string& whitespaces
        );
        std::vector<std::string> explode(const std::string& str, char delim);
        std::string trim(const std::string& str, const std::string& whitespaces);
};

#endif
