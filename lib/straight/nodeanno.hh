#ifndef straight_nodeanno_hh
#define straight_nodeanno_hh

#include "nodeindex.hh"

namespace straight {

class NodeAnno
{
private:
    const TNodeIndex descCount;
    const TNodeIndex preorder;
    bool light;

public:
    NodeAnno(TNodeIndex desc_count, TNodeIndex preorder):
        descCount(desc_count),
	preorder(preorder),
	light(true)
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

    bool is_light() const
    {
        return light;
    }

    void set_heavy()
    {
        light = false;
    }
};

}

#endif
