#include "treescore.hh"
#include <stdexcept>
#include <utility>

#define NOTRACE
#include "trace.hh"

namespace plastic {

const Answer *TreeScore::get(xmlNodePtr f, xmlNodePtr g) const
{
    TMap::const_iterator i = score.find(TMap::key_type(f, g));
    TRACE1("TreeScore: " << ((i == score.end()) ? "not found " : "found ") << f << ", " << g);
    return (i == score.end()) ? 0 : &(i->second);
}

void TreeScore::set(xmlNodePtr f, xmlNodePtr g, const Answer &a)
{
    TRACE1("TreeScore: " << f << ", " << g << " := " << a);
    std::pair<TMap::iterator, bool> inres = score.insert(
        TMap::value_type(TMap::key_type(f, g), a));
    if (!inres.second)
    {
        TRACE1("TreeScore: " << f << ", " << g << " had " << inres.first->second);
        if (a.get_score() >= inres.first->second.get_score())
	{
	    inres.first->second = a;
	}
	else
	{
	    TRACE1("TreeScore: cached score can't go down - update ignored");
	}
    }
}

}
