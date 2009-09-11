#ifndef systematic_smallenum_hh
#define systematic_smallenum_hh

#include "envelope.hh"
#include "nodeindex.hh"
#include "smallenumbase.hh"

namespace systematic {

class SmallEnum : private Envelope<SmallEnumBase>
{
public:
    SmallEnum();
    SmallEnum(INode *root, bool twist);

    TNodeIndex get_max_leaf() const
    {
        return get_letter()->get_max_leaf();
    }

    TNodeIndex get_max_size(TNodeIndex n) const
    {
        return get_letter()->get_max_size(n);
    }

    SmallEnumBase::TRange get_range(TNodeIndex n, TNodeIndex sz) const
    {
        return get_letter()->get_range(n, sz);
    }
};

}

#endif
