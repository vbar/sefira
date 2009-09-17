#include "graphedge.hh"

namespace plastic
{

GraphEdge::GraphEdge(const GraphPoint &tail, const GraphPoint &head):
    tail(tail),
    head(head)
{
}

std::ostream &operator<<(std::ostream &os, const GraphEdge &e)
{
    os << e.get_tail() << " -> " << e.get_head();
    return os;
}

}
