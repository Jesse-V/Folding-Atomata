
#ifndef ATOM_HPP
#define ATOM_HPP

#include <string>
#include <memory>

class Atom
{
    private:
        std::string symbol_;
        int atomicNumber_;
        float charge_, radius_, mass_;
};

typedef std::shared_ptr<Atom> AtomPtr;

#endif
