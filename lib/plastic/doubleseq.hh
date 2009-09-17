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
    const TNodeIndex arraySize;

public:
    DoubleSeq(xmlNodePtr g, const TreeAnno &anno);
    ~DoubleSeq();

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
  
    // not implemented
    DoubleSeq(const DoubleSeq &other);
    DoubleSeq &operator=(const DoubleSeq &other);
};

}

#endif
