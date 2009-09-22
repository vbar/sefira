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

void TreeScore::insert(xmlNodePtr f, xmlNodePtr g, const Answer &a)
{
    TRACE1("enter TreeScore::insert(" << f << ", " << g << ", " << a << ')');
    std::pair<TMap::iterator, bool> inres = score.insert(
        TMap::value_type(TMap::key_type(f, g), a));
    if (!inres.second)
    {
        TRACE1("TreeScore: " << f << ", " << g << " had " << inres.first->second << " - no update");
    }
}

void TreeScore::set(xmlNodePtr f, xmlNodePtr g, const Answer &a)
{
    TRACE1("enter TreeScore::set(" << f << ", " << g << ", " << a << ')');
    std::pair<TMap::iterator, bool> inres = score.insert(
        TMap::value_type(TMap::key_type(f, g), a));
    if (!inres.second)
    {
        TRACE1("TreeScore: " << f << ", " << g << " had " << inres.first->second);
        if (a.get_score() < inres.first->second.get_score())
	{
	    throw std::logic_error("cached score can't go down");
	}

	inres.first->second = a;
    }
}

}
