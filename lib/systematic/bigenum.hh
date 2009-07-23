#ifndef systematic_bigenum_hh
#define systematic_bigenum_hh

#include <ostream>
#include <vector>
#include "forrest.hh"
#include "nodeindex.hh"

namespace systematic {

class BigEnum
{
    friend std::ostream &operator<<(std::ostream &os, const BigEnum &be);
protected:
    typedef std::vector<Forrest> TEnum;

    bool emptyEdge;
    TEnum seq;

public:
    BigEnum();
    virtual ~BigEnum();

    TNodeIndex get_size() const;

    Forrest get(TNodeIndex idx);
};

std::ostream &operator<<(std::ostream &os, const BigEnum &be);

}

#endif

