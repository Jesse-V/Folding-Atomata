
#include "ProteinAnalysis.hpp"
#include "Viewer/SlotViewer.hpp"
#include <algorithm>
#include <thread>
#include <iterator>
#include <iostream>

typedef std::vector<std::vector<std::vector<ProteinAnalysis::Bucket>>> BucketMap;
typedef std::vector<std::vector<AtomPtr>> AtomGroups;


ProteinAnalysis::ProteinAnalysis(const TrajectoryPtr& trajectory) :
    trajectory_(trajectory)
{}



void ProteinAnalysis::fixProteinSplits()
{
    std::cout << "[concurrent] Analyzing protein for splits..." << std::endl;

    BucketMap bucketMap = getBucketMap();
    assignGroups(bucketMap);
    auto groups = getGroups(bucketMap);
    fixGroups(groups);

    std::cout << "[concurrent] ...done analyzing protein." << std::endl;
}



BucketMap ProteinAnalysis::getBucketMap()
{
    std::cout << "[concurrent] Hashing atoms into buckets of size " <<
        BOND_LENGTH << "... " << std::endl;

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

    BucketMap bucketMap;
    for_each (ATOMS.begin(), ATOMS.end(),
        [&](const AtomPtr& atom)
        {
            auto position = snapshotZero[atom];
            auto x = (std::size_t)((position.x - smallestX) / BOND_LENGTH);
            auto y = (std::size_t)((position.y - smallestY) / BOND_LENGTH);
            auto z = (std::size_t)((position.z - smallestZ) / BOND_LENGTH);

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
    std::cout << "[concurrent] ...done hashing into buckets. Took " <<
        (diff / 1000.0f) << "ms" << std::endl;

    return bucketMap;
}



void ProteinAnalysis::assignGroups(BucketMap& bucketMap)
{
    std::cout << "[concurrent] Identifying groups..." << std::endl;
    int groupID = 0;

    using namespace std::chrono;
    auto start = steady_clock::now();

    while (true)
    {
        auto pos = getUnassignedBucket(bucketMap);
        if (pos.size() < 3)
            break;

        std::cout << "[concurrent] Assigning " << groupID << "... " << std::endl;
        assignGroup(bucketMap, pos[0], pos[1], pos[2], groupID);
        groupID++;
        std::cout << "[concurrent] ...done assigning group." << std::endl;
    }

    auto diff = duration_cast<microseconds>(steady_clock::now() - start).count();
    std::cout << "[concurrent] ...done identifying groups. Took " <<
        (diff / 1000.0f) << "ms" << std::endl;
}



void ProteinAnalysis::assignGroup(BucketMap& map, int x, int y, int z, int id)
{
    if (x < 0 || y < 0 || z < 0)
        return;

    if (map.size() <= x || map[x].size() <= y || map[x][y].size() <= z)
        return;

    if (map[x][y][z].groupID >= 0)
        return;

    map[x][y][z].groupID = id;

    for (int a = -1; a <= 1; a++)
        for (int b = -1; b <= 1; b++)
            for (int c = -1; c <= 1; c++)
                assignGroup(map, x + a, y + b, z + c, id);
}



AtomGroups ProteinAnalysis::getGroups(const BucketMap& bucketMap)
{
    AtomGroups groups;
    std::cout << "[concurrent] Assembling groups... " << std::endl;

    for (std::size_t x = 0; x < bucketMap.size(); x++)
    {
        for (std::size_t y = 0; y < bucketMap[x].size(); y++)
        {
            for (std::size_t z = 0; z < bucketMap[x][y].size(); z++)
            {
                auto bucket = bucketMap[x][y][z];
                if (groups.size() <= bucket.groupID)
                    groups.resize(bucket.groupID + 1);

                auto group = groups[bucket.groupID];
                group.insert(group.end(), bucket.atoms.begin(), bucket.atoms.end());
            }
        }
    }

    std::cout << "[concurrent] ...done assembling groups." << std::endl;

    return groups;
}



void ProteinAnalysis::fixGroups(const AtomGroups& groups)
{
    //todo: needs implementation
}



std::vector<int> ProteinAnalysis::getUnassignedBucket(const BucketMap& map)
{
    for (std::size_t x = 0; x < map.size(); x++)
        for (std::size_t y = 0; y < map[x].size(); y++)
            for (std::size_t z = 0; z < map[x][y].size(); z++)
                if (map[x][y][z].groupID < 0)
                    return {(int)x, (int)y, (int)z};
    return {};
}
