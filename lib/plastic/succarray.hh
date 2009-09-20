#ifndef plastic_succarray_hh
#define plastic_succarray_hh

#include <iostream>
#include "graphedge.hh"
#include "graphpoint.hh"
#include "nodeindex.hh"
#include "shortmap.hh"

namespace plastic {

class SuccArray
{
private:
    typedef ShortMap<GraphEdge> TItem;
    TItem *array;
    const TNodeIndex arraySize;
    const TNodeIndex keyIndex;

public:
    friend std::ostream &operator<<(std::ostream &os, const SuccArray &sa);

    SuccArray(TNodeIndex array_size, TNodeIndex key_index);
    ~SuccArray();

    GraphEdge *pred(TNodeIndex idx, unsigned short key) const;

    GraphEdge *succ(TNodeIndex idx, unsigned short key) const;

    void set(TNodeIndex idx, const GraphEdge &e);

    void erase(TNodeIndex idx, unsigned short key);

    void erase(TNodeIndex idx, const GraphEdge &e)
    {
        erase(idx, e.get_head().get_coord(keyIndex));
    }

private:
    TNodeIndex rebase(TNodeIndex idx) const;

    void dump(std::ostream &os, TNodeIndex i) const;

    // not implemented
    SuccArray(const SuccArray &other);
    SuccArray &operator=(const SuccArray &other);
};

std::ostream &operator<<(std::ostream &os, const SuccArray &sa);

}

#endif
