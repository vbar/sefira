#ifndef straight_nodeanno_hh
#define straight_nodeanno_hh

#include "nodeindex.hh"

namespace straight {

class NodeAnno
{
private:
    const TNodeIndex descCount;
    bool light;

public:
    NodeAnno(TNodeIndex desc_count):
        descCount(desc_count), light(true)
    {
    }

    TNodeIndex get_desc_count() const
    {
        return descCount;
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
