#ifndef systematic_smallenum_hh
#define systematic_smallenum_hh

#include <map>
#include <ostream>
#include <set>
#include <utility>
#include "forrest.hh"
#include "nodeindex.hh"

namespace systematic {

class INode;

class SmallEnum
{
    friend std::ostream &operator<<(std::ostream &os, const SmallEnum &se);
protected:
    typedef std::map<INode *, TNodeIndex> TLeafMap;
    typedef std::set<Forrest> TKnownSet;

public:
    typedef std::pair<TNodeIndex, TNodeIndex> TIndex;
    typedef std::multimap<TIndex, Forrest> TMatrix;
    typedef TMatrix::const_iterator TConstIterator;
    typedef std::pair<TConstIterator, TConstIterator> TRange;

protected:
    TNodeIndex leafCount;
    TNodeIndex *maxSize;
    TMatrix matrix;

public:
    SmallEnum();
    virtual ~SmallEnum();

    TNodeIndex get_max_leaf() const
    {
        return leafCount;
    }

    TNodeIndex get_max_size(TNodeIndex n) const;

    TRange get_range(TNodeIndex n, TNodeIndex sz) const;

protected:
    void cond_add_index(const Forrest &forrest, INode *leaf,
	TNodeIndex sz, const TLeafMap &map, TKnownSet &set);

    static void add_leaf(INode *node, TLeafMap &map);
};

std::ostream &operator<<(std::ostream &os, const SmallEnum &se);

}

#endif
