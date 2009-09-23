#include "forrestscore.hh"
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#define NOTRACE
#include "trace.hh"

namespace plastic {

RelResult ForrestScore::get(const GraphEdge &e) const
{
    TMap::const_iterator i = scoreMap.find(e);
    if (i == scoreMap.end())
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
    std::pair<TMap::iterator, bool> inres = scoreMap.insert(
        TMap::value_type(e, r));
    if (!inres.second)
    {
	throw std::invalid_argument("edge already inserted");
    }
}

std::ostream &operator<<(std::ostream &os, const ForrestScore &fs)
{
    os << "{\n";
    std::string delim = "\t";
    for (ForrestScore::TMap::const_iterator iter = fs.scoreMap.begin();
	 iter != fs.scoreMap.end();
	 ++iter)
    {
        os << delim;
	delim = ",\n\t";
        os << iter->first << ": " << iter->second.get_score();
    }

    os << "\n}";

    return os;
}

}
