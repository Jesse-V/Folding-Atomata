
#include "Trajectory.hpp"


Trajectory::Trajectory(const std::shared_ptr<Topology> topology) :
    topology_(topology)
{}



void Trajectory::addSnapshot(const SnapshotPtr& newSnapshot)
{
    snapshots_.push_back(newSnapshot);
}
