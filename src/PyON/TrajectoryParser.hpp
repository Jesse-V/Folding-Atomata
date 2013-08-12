
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

        
};

#endif
