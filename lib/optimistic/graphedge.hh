#ifndef optimistic_graphedge_hh
#define optimistic_graphedge_hh

#include <iostream>
#include <utility>
#include "nodeindex.hh"

namespace optimistic {

typedef std::pair<TNodeIndex, TNodeIndex> TGraphPoint;

class GraphEdge
{
private:
    TGraphPoint tail;
    TGraphPoint head;

public:
    GraphEdge(TGraphPoint tail, TGraphPoint head);

    bool operator<(const GraphEdge &other) const
    {
        return (tail < other.tail) ||
	    ((tail == other.tail) && (head < other.head));
    }

    bool operator==(const GraphEdge &other) const
    {
        return (tail == other.tail) && (head == other.head);
    }

    TGraphPoint get_tail() const { return tail; }

    TNodeIndex tail1() const { return tail.first; }
    TNodeIndex tail2() const { return tail.second; }
    TNodeIndex head1() const { return head.first; }
    TNodeIndex head2() const { return head.second; }
};

std::ostream &operator<<(std::ostream &os, const GraphEdge &e);

}

#endif
