#include "lcsmemo.hh"
#include <iomanip>
#include <iostream>

#define NOTRACE
#include "trace.hh"

namespace straight {

const Answer *LcsMemo::get(TKernel f, TKernel g) const
{
    TMap::const_iterator i = memo.find(TMap::key_type(f, g));
    TRACE1(((i == memo.end()) ? "not found " : "found ") << f << ", " << g);
    return (i == memo.end()) ? 0 : &(i->second);
}

void LcsMemo::insert(TKernel f, TKernel g, Answer a)
{
    TRACE1("caching " << f << ", " << g);
    memo.insert(TMap::value_type(TMap::key_type(f, g), a));
}

}
