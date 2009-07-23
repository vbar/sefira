#include "graphedge.hh"
#include <stdexcept>

namespace optimistic {

GraphEdge::GraphEdge(TGraphPoint tail, TGraphPoint head):
    tail(tail),
    head(head)
{
    if ((tail.first >= head.first) ||
	(tail.second >= head.second))
    {
	throw std::invalid_argument("illegal edge");
    }
}

std::ostream &operator<<(std::ostream &os, const GraphEdge &e)
{
    os << '(' << e.tail1() << ", " << e.tail2() << ") -> (";
    os << e.head1() << ", " << e.head2() << ')';
    return os;
}

}
