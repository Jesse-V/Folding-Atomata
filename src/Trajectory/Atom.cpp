
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

#include "Atom.hpp"


Atom::Atom(const std::string& symbol, int atomicNumber, float charge,
            float radius, float mass) :
    symbol_(symbol), atomicNumber_(atomicNumber),
    charge_(charge), radius_(radius), mass_(mass)
{}



glm::vec3 Atom::getColor()
{
    // http://en.wikipedia.org/wiki/CPK_coloring

    /* FAHViewer uses:
        hydrogen == light gray
        carbon == dark gray
        nitrogen == blue
    */

    switch (getElement())
    {
        case 'H' : //hydrogen
            return glm::vec3(0.933f, 0.933f, 0.933f); //238, 238, 238

        case 'C' : //carbon
            return glm::vec3(0.133f, 0.133f, 0.133f); //34, 34, 34

        case 'N' : //nitrogen
            return glm::vec3(0.133f, 0.2f, 1.0f); //34, 51, 255

        case 'O' : //oxygen
            return glm::vec3(1.0f, 0.133f, 0.0f); //255, 34, 0

        case 'S' : //sulfur
            return glm::vec3(0.866f, 0.866f, 0.0f); //221, 221, 0

        default : //all other elements
            return glm::vec3(0.866f, 0.466f, 1.0f); //221, 119, 255
    }
}



char Atom::getElement()
{
    return getSymbol().at(0);
}



std::string Atom::getSymbol()
{
    return symbol_;
}



int Atom::getAtomicNumber()
{
    return atomicNumber_;
}



float Atom::getCharge()
{
    return charge_;
}



float Atom::getRadius()
{
    return radius_;
}



float Atom::getMass()
{
    return mass_;
}



float Atom::getElectronShellCount()
{
    switch (getElement())
    {
        case 'H' :
            return 1;

        case 'C' :
            return 2;

        case 'N' :
            return 2;

        case 'O' :
            return 2;

        case 'S' :
            return 3;

        default :
            return 4;
    }
}
