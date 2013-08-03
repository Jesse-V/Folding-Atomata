
#ifndef TOPOLOGY_HPP
#define TOPOLOGY_HPP

#include "Atom.hpp"
#include "Bond.hpp"
#include <memory>
#include <vector>

class Topology
{
    public:
        Topology(const std::vector<AtomPtr>& atoms,
                 const std::vector<BondPtr>& bonds
        );
        std::vector<AtomPtr> getAtoms();
        std::vector<BondPtr> getBonds();
        
    private:
        std::vector<AtomPtr> atoms_;
        std::vector<BondPtr> bonds_;
};

typedef std::shared_ptr<Topology> TopologyPtr;

#endif
