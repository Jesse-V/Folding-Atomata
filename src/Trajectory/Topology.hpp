
#ifndef TOPOLOGY_HPP
#define TOPOLOGY_HPP

#include "Atom.hpp"
#include "Bond.hpp"
#include <memory>
#include <vector>

class Topology
{
    private:
        std::vector<AtomPtr> atoms_;
        std::vector<BondPtr> bonds_;
};

typedef std::shared_ptr<Topology> TopologyPtr;

#endif
