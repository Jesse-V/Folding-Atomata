
#include "Topology.hpp"


Topology::Topology(const std::vector<AtomPtr>& atoms, const std::vector<BondPtr>& bonds) :
    atoms_(atoms), bonds_(bonds)
{}



std::vector<AtomPtr> Topology::getAtoms()
{
    return atoms_;
}



std::vector<BondPtr> Topology::getBonds()
{
    return bonds_;
}
