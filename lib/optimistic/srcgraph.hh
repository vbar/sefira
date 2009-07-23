#ifndef optimistic_srcgraph_hh
#define optimistic_srcgraph_hh

#include <iostream>
#include <map>
#include <vector>
#include "graphedge.hh"
#include "nodeindex.hh"

namespace optimistic {

class SrcGraph
{
public:
    typedef std::vector<GraphEdge> TGroup;

private:
    typedef std::map<TNodeIndex, TGroup> TMap;

    static TGroup emptyGroup;

    TMap headMap;
    TMap tailMap;

public:
    const TGroup &head_group(TNodeIndex first) const
    {
        return get_group(headMap, first);
    }

    const TGroup &tail_group(TNodeIndex first) const
    {
        return get_group(tailMap, first);
    }

    void insert(const GraphEdge &e)
    {
        insert_group(headMap, e.head1(), e);
	insert_group(tailMap, e.tail1(), e);
    }

    void dump_head(std::ostream &os) const
    {
        dump(os, headMap);
    }

    void dump_tail(std::ostream &os) const
    {
        dump(os, tailMap);
    }

private:
    static const TGroup &get_group(const TMap &m, TNodeIndex first);

    static void insert_group(TMap &m, TNodeIndex key, const GraphEdge &e);

    void dump(std::ostream &os, const TMap &m) const;
};

std::ostream &operator<<(std::ostream &os, const SrcGraph &sg);

}

#endif
