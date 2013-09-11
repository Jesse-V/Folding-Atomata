
#include "ProteinAnalysis.hpp"
#include <algorithm>
#include <thread>
#include <iostream>

typedef std::unordered_multimap<ProteinAnalysis::Bucket, AtomPtr,
    ProteinAnalysis::Bucket> BucketMap;
typedef std::unordered_multimap<AtomPtr, AtomPtr> NeighborList;


ProteinAnalysis::ProteinAnalysis(const TrajectoryPtr& trajectory) :
    trajectory_(trajectory)
{
    //fetch an unordered_map of atoms to their position in snapshot zero

    std::unordered_map<AtomPtr, AtomPtr> mapA;
    std::unordered_map<AtomPtr, glm::vec3> mapB;
}



void ProteinAnalysis::fixProteinSplits()
{
    std::cout << "[concurrent] Analyzing protein for splits..." << std::endl;

    BucketMap bucketMap = getBucketMap();
    NeighborList neighborList = getNeighborList(bucketMap);


}



BucketMap ProteinAnalysis::getBucketMap()
{
    std::cout << "[concurrent] hashing atoms into cube buckets... ";
    BucketMap bucketMap;

    auto atoms = trajectory_->getTopology()->getAtoms();
    auto snapshotZero = trajectory_->getSnapshot(0);

    //hash each atom into some bucket according to its position
    for (std::size_t j = 0; j < atoms.size(); j++)
    {
        auto pos = snapshotZero->getPosition((int)j); //j only used here
        Bucket b;
        b.x = (int)(pos.x / BOND_LENGTH);
        b.y = (int)(pos.y / BOND_LENGTH);
        b.z = (int)(pos.z / BOND_LENGTH);

        bucketMap.insert(BucketMap::value_type(b, atoms[j]));
    }

    std::cout << "done." << std::endl;
    return bucketMap;
}



NeighborList ProteinAnalysis::getNeighborList(const BucketMap& bucketMap)
{
    std::cout << "[concurrent] Computing neighbor list for each atom... ";
    NeighborList neighborList;

    for_each (bucketMap.begin(), bucketMap.end(),
        [&](const std::pair<Bucket, AtomPtr>& current)
    {
        auto bucket = current.first;
        auto atom = current.second;

        //scan surrounding 27 buckets
        for (int x = -1; x <= 1; x++)
        {
            for (int y = -1; y <= 1; y++)
            {
                for (int z = -1; z <= 1; z++)
                {
                    Bucket neighborBucket;
                    neighborBucket.x = bucket.x + x;
                    neighborBucket.y = bucket.y + y;
                    neighborBucket.z = bucket.z + z;

                    auto contents = bucketMap.equal_range(neighborBucket);
                    for_each (contents.first, contents.second,
                        [&] (const std::pair<Bucket, AtomPtr>& near)
                    {
                        auto nearbyAtom = near.second;

                        if (isWithinBondDistance(atom, nearbyAtom))
                        {
                            auto pair = NeighborList::value_type(atom, nearbyAtom);
                            neighborList.insert(pair);
                        }
                    });
                }
            }
        }
    });

    std::cout << " done." << std::endl;
    return neighborList;
}



bool ProteinAnalysis::isWithinBondDistance(const AtomPtr& a, const AtomPtr& b)
{
    //auto atomPosition = snapshotZero->getPosition(5);

    return true; //temp
}
