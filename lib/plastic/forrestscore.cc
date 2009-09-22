#include "forrestscore.hh"
#include <stdexcept>
#include <utility>

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
    TRACE1("enter ForrestScore::insert(" << e << ", " << a << ')');
    std::pair<TMap::iterator, bool> inres = score.insert(
        TMap::value_type(e, a));
    if (!inres.second)
    {
	throw std::invalid_argument("edge already inserted");
    }
}

void ForrestScore::set(const GraphEdge &e, const Answer &a)
{
    TRACE1("enter ForrestScore::set(" << e << ", " << a << ')');
    std::pair<TMap::iterator, bool> inres = score.insert(
        TMap::value_type(e, a));
    if (!inres.second)
    {
        TRACE1("ForrestScore: " << e << " had " << inres.first->second);
        if (a.get_score() < inres.first->second.get_score())
	{
	    throw std::logic_error("cached edge score can't go down");
	}

	inres.first->second = a;
    }
}

}
