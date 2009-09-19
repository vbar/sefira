#include "forrestscore.hh"

#define NOTRACE
#include "trace.hh"

namespace plastic {

const Answer *ForrestScore::get(const GraphEdge &e) const
{
    TMap::const_iterator i = score.find(e);
    TRACE1("ForrestScore: " << ((i == score.end()) ? "not found " : "found ") << e);
    return (i == score.end()) ? 0 : &(i->second);
}

void ForrestScore::insert(const GraphEdge &e, const Answer &a)
{
    TRACE1("caching " << e);
    score.insert(TMap::value_type(e, a));
}

}
