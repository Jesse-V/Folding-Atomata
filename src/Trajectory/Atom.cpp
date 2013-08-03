
#include "Atom.hpp"


Atom::Atom(const std::string& symbol, int atomicNumber, float charge, 
            float radius, float mass) : 
    symbol_(symbol), atomicNumber_(atomicNumber), charge_(charge), radius_(radius), mass_(mass)
{}



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
