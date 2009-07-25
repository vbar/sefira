#ifndef systematic_bigenumbase_hh
#define systematic_bigenumbase_hh

#include <ostream>
#include <vector>
#include "forrest.hh"
#include "nodeindex.hh"

namespace systematic {

class BigEnumBase
{
    friend std::ostream &operator<<(std::ostream &os, const BigEnumBase &be);
protected:
    typedef std::vector<Forrest> TEnum;

    bool emptyEdge;
    TEnum seq;

public:
    BigEnumBase();
    virtual ~BigEnumBase();

    TNodeIndex get_size() const;

    Forrest get(TNodeIndex idx) const;
};

std::ostream &operator<<(std::ostream &os, const BigEnumBase &be);

}

#endif

