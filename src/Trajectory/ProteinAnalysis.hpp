
#ifndef PROTEIN_ANALYSIS
#define PROTEIN_ANALYSIS

#include "Trajectory.hpp"
#include <unordered_map>

class ProteinAnalysis
{
    public:
        const float BOND_LENGTH = 2;

        struct Bucket
        {
            int x, y, z;

            bool operator() (const Bucket& b) const { return isEqualTo(b); }
            bool operator==(const Bucket& b) const  { return isEqualTo(b); }

            bool isEqualTo(const Bucket& other) const
            {
                return x == other.x && y == other.y && z == other.z;
            }
        };

        typedef std::unordered_multimap<Bucket, AtomPtr, Bucket> BucketMap;
        typedef std::unordered_multimap<AtomPtr, AtomPtr> NeighborList;

    public:
        ProteinAnalysis(const TrajectoryPtr& trajectory);
        void fixProteinSplits();
        BucketMap getBucketMap();
        NeighborList getNeighborList(const BucketMap& bucketMap);
        bool isWithinBondDistance(const AtomPtr& a, const AtomPtr& b);

    private:
        TrajectoryPtr trajectory_;
};



#endif
