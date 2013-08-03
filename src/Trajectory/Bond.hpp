
#ifndef BOND_HPP
#define BOND_HPP

#include <memory>

class Bond
{
    public:
        Bond(int atomA, int atomB);
        int getAtomA();
        int getAtomB();
        
    private:
        int atomA_, atomB_;
};

typedef std::shared_ptr<Bond> BondPtr;

#endif
