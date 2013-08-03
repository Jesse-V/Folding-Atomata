
#ifndef BOND_HPP
#define BOND_HPP

#include <memory>

class Bond
{
    private:
        int atomA_, atomB_;
};

typedef std::shared_ptr<Bond> BondPtr;

#endif
