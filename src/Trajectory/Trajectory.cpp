
#include "Trajectory.hpp"


Trajectory::Trajectory(const std::shared_ptr<Topology> topology) :
    topology_(topology)
{}



std::shared_ptr<Topology> Trajectory::getTopology()
{
    return topology_;
}



void Trajectory::addSnapshot(const SnapshotPtr& newSnapshot)
{
    snapshots_.push_back(newSnapshot);
}



SnapshotPtr Trajectory::getSnapshot(int index)
{
    return snapshots_[index];
}



int Trajectory::countSnapshots()
{
    return snapshots_.size();
}
