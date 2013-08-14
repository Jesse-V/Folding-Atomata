
#ifndef TRAJECTORY_HPP
#define TRAJECTORY_HPP

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

/*
    Trajectory
        has topology
            has list of atoms
                each has symbol, charge, radius, mass, and atomic number
            has list of bond
                each has pair of indexes, referring to connected atoms
        has list of snapshots
            each has list of xyz coordinates for each atom
*/
