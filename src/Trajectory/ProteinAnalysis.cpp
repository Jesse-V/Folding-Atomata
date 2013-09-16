
#include "ProteinAnalysis.hpp"
#include "Viewer/SlotViewer.hpp"
#include <algorithm>
#include <thread>
#include <iterator>
#include <iostream>

typedef std::vector<std::vector<std::vector<ProteinAnalysis::Bucket>>> BucketMap;

ProteinAnalysis::ProteinAnalysis(const TrajectoryPtr& trajectory) :
    trajectory_(trajectory)
{}



void ProteinAnalysis::fixProteinSplits()
{
    std::cout << "[concurrent] Analyzing protein for splits..." << std::endl;

    BucketMap bucketMap = getBucketMap();

}



BucketMap ProteinAnalysis::getBucketMap()
{
    std::cout << "[concurrent] hashing atoms into cube buckets... ";
    BucketMap bucketMap;

    const auto ATOMS = trajectory_->getTopology()->getAtoms();
    auto snapshotZero = trajectory_->getSnapshot(0);

    using namespace std::chrono;
    auto start = steady_clock::now();

    float smallestX = 0, smallestY = 0, smallestZ = 0;
    for_each (ATOMS.begin(), ATOMS.end(),
        [&](const AtomPtr& atom)
        {
            auto position = snapshotZero[atom];
            if (position.x < smallestX)
                smallestX = position.x;
            if (position.y < smallestY)
                smallestY = position.y;
            if (position.z < smallestZ)
                smallestZ = position.z;
        }
    );

    for_each (ATOMS.begin(), ATOMS.end(),
        [&](const AtomPtr& atom)
        {
            auto position = snapshotZero[atom];
            int x = (int)((position.x - smallestX) / BOND_LENGTH);
            int y = (int)((position.y - smallestY) / BOND_LENGTH);
            int z = (int)((position.z - smallestZ) / BOND_LENGTH);

            if (bucketMap.size() <= x)
                bucketMap.resize(x + 1);
            if (bucketMap[x].size() <= y)
                bucketMap[x].resize(y + 1);
            if (bucketMap[x][y].size() <= z)
                bucketMap[x][y].resize(z + 1);

            bucketMap[x][y][z].atoms.push_back(atom);
        }
    );

    auto diff = duration_cast<microseconds>(steady_clock::now() - start).count();
    std::cout << "done. Took " << (diff / 1000.0f) << "ms" << std::endl;
    return bucketMap;
}
