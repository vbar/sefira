#ifndef optimistic_doubleidx_hh
#define optimistic_doubleidx_hh

#include <iostream>
#include <utility>
#include "envelope.hh"
#include "nodeindex.hh"

namespace optimistic {

class DoubleIdx : private Envelope<std::pair<TNodeIndex, TNodeIndex> >
{
public:
    typedef TLetter TPayload;

    DoubleIdx(TNodeIndex in);
    DoubleIdx(const DoubleIdx &other);

    TNodeIndex get_in() const
    {
        return get_letter()->first;
    }

    TNodeIndex get_out() const
    {
        return get_letter()->second;
    }

    TPayload *get_payload()
    {
        return get_letter();
    }
};

std::ostream &operator<<(std::ostream &os, const DoubleIdx &d);

};

#endif
