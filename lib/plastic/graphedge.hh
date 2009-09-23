#ifndef plastic_graphedge_hh
#define plastic_graphedge_hh

#include <iostream>
#include "graphpoint.hh"

namespace plastic
{

class GraphEdge
{
private:
    GraphPoint tail;
    GraphPoint head;

public:
    GraphEdge(const GraphPoint &tail, const GraphPoint &head);

    bool operator<(const GraphEdge &other) const
    {
        return (tail < other.tail) ||
	    ((tail == other.tail) && (head < other.head));
    }

    const GraphPoint &get_tail() const
    {
        return tail;
    }

    const GraphPoint &get_head() const
    {
        return head;
    }

    GraphPoint &get_tail()
    {
        return tail;
    }

    GraphPoint &get_head()
    {
        return head;
    }
};

std::ostream &operator<<(std::ostream &os, const GraphEdge &e);

}

#endif
