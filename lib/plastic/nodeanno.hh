#ifndef plastic_nodeanno_hh
#define plastic_nodeanno_hh

#include "nodeindex.hh"

namespace plastic {

class NodeAnno
{
private:
    const TNodeIndex descCount;
    const TNodeIndex preorder;

public:
    NodeAnno(TNodeIndex desc_count, TNodeIndex preorder):
        descCount(desc_count),
	preorder(preorder)
    {
    }

    TNodeIndex get_desc_count() const
    {
        return descCount;
    }

    TNodeIndex get_preorder() const
    {
        return preorder;
    }
};

}

#endif
