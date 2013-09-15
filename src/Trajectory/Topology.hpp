
/******************************************************************************\
                     This file is part of Folding Atomata,
          a program that displays 3D views of Folding@home proteins.

                      Copyright (c) 2013, Jesse Victors

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see http://www.gnu.org/licenses/

                For information regarding this software email:
                                Jesse Victors
                         jvictors@jessevictors.com
\******************************************************************************/

#ifndef TOPOLOGY
#define TOPOLOGY

/**
    The Topology class holds a list of atoms and the bonds between them.
    An Atom is simply an instance of the Atom class, whereas a Bond is a
    std::pair of shared pointers to two of those instances. This allows
    an algorithm to quickly gain information about the two connected atoms
    without having to look them up in a list by their indexes.
**/

#include "Atom.hpp"
#include <memory>
#include <unordered_map>
#include <vector>

typedef std::pair<AtomPtr, AtomPtr> Bond;
typedef std::vector<Bond> BondList;

class Topology
{
    public:
        Topology(const std::vector<AtomPtr>& atoms,
                 const BondList& bonds
        );
        std::vector<AtomPtr> getAtoms();
        BondList getBonds();

    private:
        std::vector<AtomPtr> atoms_;
        BondList bonds_;
};

typedef std::shared_ptr<Topology> TopologyPtr;

#endif
