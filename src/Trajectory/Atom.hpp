
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

#ifndef ATOM
#define ATOM

#include "glm/glm.hpp"
#include <string>
#include <memory>

class Atom
{
    public:
        Atom(const std::string& symbol, int atomicNumber, float charge,
            float radius, float mass
        );

        glm::vec3 getColor();
        char getElement();

        std::string getSymbol();
        int getAtomicNumber();
        float getCharge();
        float getRadius();
        float getMass();

    private:
        std::string symbol_;
        int atomicNumber_;
        float charge_, radius_, mass_;
};

typedef std::shared_ptr<Atom> AtomPtr;

#endif
