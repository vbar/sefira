#ifndef plastic_doubleseq_hh
#define plastic_doubleseq_hh

#include <libxml/tree.h>
#include "nodeindex.hh"

namespace plastic {

class TreeAnno;

class DoubleSeq
{
private:
    struct Item
    {
        xmlNodePtr ystep;
        TNodeIndex euler; // 1-based
    };

    Item *array;
    TNodeIndex treeSize; // 1/2 of array size

public:
    DoubleSeq(xmlNodePtr g, TNodeIndex sz);
    DoubleSeq(const DoubleSeq &other);
    ~DoubleSeq();

    DoubleSeq &operator=(const DoubleSeq &other);

    TNodeIndex get_tree_size() const
    {
        return treeSize;
    }

    xmlNodePtr get_ystep(TNodeIndex idx) const
    {
        return array[rebase(idx)].ystep;
    }

    TNodeIndex get_euler_index(TNodeIndex idx) const
    {
        return array[rebase(idx)].euler;
    }

private:
    TNodeIndex rebase(TNodeIndex idx) const
    {
        return idx - 1;
    }

    // i is 0-based
    TNodeIndex fill(xmlNodePtr node, TNodeIndex i);
};

}

#endif
