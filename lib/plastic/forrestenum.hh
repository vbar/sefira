#ifndef plastic_forrestenum_hh
#define plastic_forrestenum_hh

#include <iostream>
#include <libxml/tree.h>
#include "nodeindex.hh"

namespace plastic {

class Decomposition;

class ForrestEnum
{
private:
    struct Item
    {
        xmlNodePtr xstep;
        bool left;
        bool tree;
    };

    Item *array;
    const TNodeIndex arraySize;

public:
    friend std::ostream &operator<<(std::ostream &os, const ForrestEnum &fe);

    ForrestEnum(xmlNodePtr f, const Decomposition &decomposition);
    ~ForrestEnum();

    bool is_left(TNodeIndex idx) const
    {
        return array[rebase(idx)].left;
    }

    // returns a valid node (never NULL)
    xmlNodePtr get_xstep(TNodeIndex idx) const
    {
        return array[rebase(idx)].xstep;
    }

    // returns the root of F(idx) when it's a tree, NULL for generic
    // forrests
    xmlNodePtr get_xtree(TNodeIndex idx) const
    {
        Item *current = array + rebase(idx);
        return current->tree ? current->xstep : 0;
    }

private:
    TNodeIndex rebase(TNodeIndex idx) const;

    void dump(std::ostream &os, TNodeIndex i) const;

    // not implemented
    ForrestEnum(const ForrestEnum &other);
    ForrestEnum &operator=(const ForrestEnum &other);
};

std::ostream &operator<<(std::ostream &os, const ForrestEnum &fe);

}

#endif
