#include "graph.hh"
#include "graphpoint.hh"
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace plastic {

Answer Graph::get(const GraphEdge &e) const
{
    TFullMap::const_iterator iter = weightMap.find(e);
    if (iter == weightMap.end())
    {
        std::stringstream ss;
	ss << "unknown edge " << e;
	throw std::invalid_argument(ss.str());
    }

    return iter->second;
}

void Graph::insert(const GraphEdge &e, const Answer &a)
{
    headMap.insert(
	TEdgeMap::value_type(
	    e.get_head().get_coord(GraphPoint::big_index_index),
	    e));
    tailMap.insert(
	TEdgeMap::value_type(
	    e.get_tail().get_coord(GraphPoint::big_index_index),
	    e));

    std::pair<TFullMap::iterator, bool> inres = weightMap.insert(
	TFullMap::value_type(e, a));
    if (!inres.second)
    {
        std::stringstream ss;
	ss << "value of edge " << e << " already inserted";
	throw std::invalid_argument(ss.str());
    }
}

std::ostream &operator<<(std::ostream &os, const Graph &g)
{
    os << "{\n";
    std::string delim = "\t";
    for (Graph::TFullMap::const_iterator iter = g.weightMap.begin();
	 iter != g.weightMap.end();
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
