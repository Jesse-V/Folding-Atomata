
#ifndef ATOM_HPP
#define ATOM_HPP

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
