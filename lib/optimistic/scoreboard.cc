#include "scoreboard.hh"
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace optimistic {

RelResult ScoreBoard::get(const GraphEdge &e) const
{
    TMap::const_iterator iter = scoreMap.find(e);
    if (iter == scoreMap.end())
    {
        std::stringstream ss;
	ss << e << " not yet scored";
	throw std::invalid_argument(ss.str());
    }

    return iter->second;
}

void ScoreBoard::insert(const GraphEdge &e, const RelResult &r)
{
    std::pair<TMap::iterator, bool> inres = scoreMap.insert(
        TMap::value_type(e, r));
    if (!inres.second)
    {
	throw std::invalid_argument("edge already inserted");
    }
}

std::ostream &operator<<(std::ostream &os, const ScoreBoard &b)
{
    os << "{\n";
    std::string delim = "\t";
    for (ScoreBoard::TMap::const_iterator iter = b.scoreMap.begin();
	 iter != b.scoreMap.end();
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
