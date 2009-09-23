#include "forrestscore.hh"
#include <sstream>
#include <stdexcept>
#include <utility>

#define NOTRACE
#include "trace.hh"

namespace plastic {

RelResult ForrestScore::get(const GraphEdge &e) const
{
    TMap::const_iterator i = score.find(e);
    if (i == score.end())
    {
        std::stringstream ss;
	ss << e << " not yet scored";
	throw std::invalid_argument(ss.str());
    }

    return i->second;
}

void ForrestScore::insert(const GraphEdge &e, const RelResult &r)
{
    TRACE1("enter ForrestScore::insert(" << e << ", " << r << ')');
    std::pair<TMap::iterator, bool> inres = score.insert(
        TMap::value_type(e, r));
    if (!inres.second)
    {
	throw std::invalid_argument("edge already inserted");
    }
}

}
