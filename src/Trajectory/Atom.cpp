
#include "Atom.hpp"


Atom::Atom(const std::string& symbol, int atomicNumber, float charge, 
            float radius, float mass) : 
    symbol_(symbol), atomicNumber_(atomicNumber), charge_(charge), radius_(radius), mass_(mass)
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
