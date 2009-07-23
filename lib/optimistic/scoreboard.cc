#include "scoreboard.hh"
#include <string>
#include <utility>

namespace optimistic {

RelResult ScoreBoard::get(const GraphEdge &e) const
{
    TMap::const_iterator iter = scoreMap.find(e);
    if (iter == scoreMap.end())
    {
	return RelResult();
    }

    return iter->second;
}

void ScoreBoard::insert(const GraphEdge &e, RelResult r)
{
    scoreMap.insert(TMap::value_type(e, r));
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
