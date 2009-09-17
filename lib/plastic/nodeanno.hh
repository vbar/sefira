#ifndef plastic_nodeanno_hh
#define plastic_nodeanno_hh

#include "nodeindex.hh"

namespace plastic {

class NodeAnno
{
private:
    const TNodeIndex descCount;

public:
    NodeAnno(TNodeIndex desc_count):
        descCount(desc_count)
    {
    }

    TNodeIndex get_desc_count() const
    {
        return descCount;
    }
};

}

#endif
