#include "treescore.hh"
#include <stdexcept>
#include <utility>

#define NOTRACE
#include "trace.hh"

namespace plastic {

const RelResult *TreeScore::get(xmlNodePtr f, xmlNodePtr g) const
{
    TMap::const_iterator i = score.find(TMap::key_type(f, g));
    TRACE1("TreeScore: " << ((i == score.end()) ? "not found " : "found ") << f << ", " << g);
    return (i == score.end()) ? 0 : &(i->second);
}

void TreeScore::update(xmlNodePtr f, xmlNodePtr g, const RelResult &r)
{
    TRACE1("enter TreeScore::update(" << f << ", " << g << ", " << r << ')');
    std::pair<TMap::iterator, bool> inres = score.insert(
        TMap::value_type(TMap::key_type(f, g), r));
    if (!inres.second)
    {
        TRACE1("TreeScore: " << f << ", " << g << " had " << inres.first->second);
        if (r.get_score() > inres.first->second.get_score())
	{
	    inres.first->second = r;
	}
    }
}

}
