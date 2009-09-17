#ifndef plastic_graphpoint_hh
#define plastic_graphpoint_hh

#include <iostream>
#include "nodeindex.hh"

namespace plastic
{

class GraphPoint
{
public:
    static const TNodeIndex dimension = 3;

private:
    TNodeIndex point[dimension];

public:
    static const TNodeIndex big_index_index = 0;
    static const TNodeIndex small_first_index = 1;
    static const TNodeIndex small_last_index = 2;

    friend std::ostream &operator<<(std::ostream &os, const GraphPoint &pt);

    GraphPoint(TNodeIndex big_index, TNodeIndex small_first,
	       TNodeIndex small_last);

    bool operator==(const GraphPoint &other) const;

    bool operator<(const GraphPoint &other) const;

    TNodeIndex get_coord(TNodeIndex index) const
    {
        return point[index];
    }
};

std::ostream &operator<<(std::ostream &os, const GraphPoint &pt);

}

#endif
