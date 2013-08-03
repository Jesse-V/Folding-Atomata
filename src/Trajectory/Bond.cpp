
#include "Bond.hpp"


Bond::Bond(int atomA, int atomB) :
    atomA_(atomA), atomB_(atomB)
{}



int Bond::getAtomA()
{
    return atomA_;
}



int Bond::getAtomB()
{
    return atomB_;
}
