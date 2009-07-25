#ifndef systematic_bigenum_hh
#define systematic_bigenum_hh

#include "bigenumbase.hh"
#include "envelope.hh"
#include "nodeindex.hh"

namespace systematic {

class Forrest;
class INode;

class BigEnum : private Envelope<BigEnumBase>
{
public:
    BigEnum(INode *parent, bool twist);

    TNodeIndex get_size() const
    {
        return get_letter()->get_size();
    }

    Forrest get(TNodeIndex idx) const
    {
        return get_letter()->get(idx);
    }
};

}

#endif
