
#ifndef TRAJECTORY_PARSER_HPP
#define TRAJECTORY_PARSER_HPP

#include "../Trajectory/Trajectory.hpp"

class TrajectoryParser
{
    public:
        TrajectoryPtr parse(const std::string& trajectoryStr);

    private:
        TopologyPtr parseTopology(const std::string& topologyStr);
        SnapshotPtr parseSnapshot(const std::string& snapshotStr);
        std::vector<AtomPtr> parseAtoms(const std::string& topologyStr);
        AtomPtr parseAtom(const std::string& atomStr);
        std::vector<BondPtr> parseBonds(const std::string& topologyStr);
        BondPtr parseBond(const std::string& bondStr);
        glm::vec3 parsePosition(const std::string& positionStr);

        std::string getBetween(const std::string& str, 
                               const std::string& header,
                               const std::string& footer, int start = 0
        );
        std::vector<std::string> explode(const std::string& str, char delim);
        std::string trim(const std::string& str, const std::string& whitespaces);

        //static std::vector<std::string> explode(const std::string& str, char delim);
};

#endif
