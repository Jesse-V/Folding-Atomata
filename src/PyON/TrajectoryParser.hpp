
#ifndef TRAJECTORY_PARSER_HPP
#define TRAJECTORY_PARSER_HPP

#include "../Trajectory/Trajectory.hpp"

class TrajectoryParser
{
    public:
        static TrajectoryPtr parse(const std::string& trajectoryStr);
        static SnapshotPtr parseSnapshot(const std::string& snapshotStr);

    private:
        TrajectoryParser() {} //prevents instantiation
        static TopologyPtr parseTopology(const std::string& topologyStr);
        static std::vector<AtomPtr> parseAtoms(const std::string& topologyStr);
        static AtomPtr parseAtom(const std::string& atomStr);
        static std::vector<BondPtr> parseBonds(const std::string& topologyStr);
        static BondPtr parseBond(const std::string& bondStr);
        static glm::vec3 parsePosition(const std::string& positionStr);

        
};

#endif
