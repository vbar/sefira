#include "graphpoint.hh"
#include <stdexcept>
#include <string>

namespace plastic
{

GraphPoint::GraphPoint(TNodeIndex big_index, TNodeIndex small_first,
		       TNodeIndex small_last)
{
    if (!small_first) {
	throw std::invalid_argument("zero small index");
    }

    if (small_last < small_first) {
	throw std::invalid_argument("small index range ends before beginning");
    }

    point[big_index_index] = big_index;
    point[small_first_index] = small_first;
    point[small_last_index] = small_last;
}

bool GraphPoint::operator==(const GraphPoint &other) const
{
    for (int i = 0; i < dimension; ++i) {
        if (point[i] != other.point[i]) {
	    return false;
	}
    }

    return true;
}

bool GraphPoint::operator<(const GraphPoint &other) const
{
    for (int i = 0; i < dimension; ++i) {
        if (point[i] != other.point[i]) {
	    return point[i] < other.point[i];
	}
    }

    return false;
}

std::ostream &operator<<(std::ostream &os, const GraphPoint &pt)
{
    std::string delim("(");
    for (int i = 0; i < GraphPoint::dimension; ++i) {
        os << delim << pt.point[i];
	delim = ", ";
    }

    os << ')';
    return os;
}

}
