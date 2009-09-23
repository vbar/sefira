#include "graph.hh"
#include "graphpoint.hh"
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace plastic {

RelResult Graph::get(const GraphEdge &e) const
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

void Graph::insert(const GraphEdge &e, const RelResult &r)
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
	TFullMap::value_type(e, r));
    if (!inres.second)
    {
        std::stringstream ss;
	ss << "value of edge " << e << " already inserted";
	throw std::invalid_argument(ss.str());
    }
}

#if 0
static void dump(std::ostream &os, const Graph::TEdgeMap &edge_map)
{
    os << "{\n";
    std::string delim = "\t";
    for (Graph::TEdgeMap::const_iterator iter = edge_map.begin();
	 iter != edge_map.end();
	 ++iter)
    {
        os << delim;
	delim = ",\n\t";
        os << iter->first << ": " << iter->second;
    }
  
    os << " }";
}
#endif

std::ostream &operator<<(std::ostream &os, const Graph &g)
{
#if 0
    os << "{\n head = ";
    dump(os, g.headMap);

    os << "\n tail = ";
    dump(os, g.tailMap);

    os << "\n weight = {\n";
#else
    os << "{\n weight = {\n";
#endif
    std::string delim = "\t";
    for (Graph::TFullMap::const_iterator iter = g.weightMap.begin();
	 iter != g.weightMap.end();
	 ++iter)
    {
        os << delim;
	delim = ",\n\t";
        os << iter->first << ": " << iter->second.get_score();
    }

    os << "\n }\n}";

    return os;
}

}
