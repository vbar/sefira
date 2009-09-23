#ifndef plastic_graph_hh
#define plastic_graph_hh

#include <map>
#include "graphedge.hh"
#include "nodeindex.hh"
#include "relresult.hh"

namespace plastic {

class Graph
{
public:
    typedef std::multimap<TNodeIndex, GraphEdge> TEdgeMap;
    typedef TEdgeMap::const_iterator TConstIterator;
    typedef std::pair<TConstIterator, TConstIterator> TRange;

private:
    typedef std::map<GraphEdge, RelResult> TFullMap;

    TEdgeMap headMap;
    TEdgeMap tailMap;  
    TFullMap weightMap;

public:
    friend std::ostream &operator<<(std::ostream &os, const Graph &g);

    TRange get_head_range(TNodeIndex idx) const
    {
        return headMap.equal_range(idx);
    }

    TRange get_tail_range(TNodeIndex idx) const
    {
        return tailMap.equal_range(idx);
    }

    RelResult get(const GraphEdge &e) const;

    void insert(const GraphEdge &e, const RelResult &r);
};

}

#endif
