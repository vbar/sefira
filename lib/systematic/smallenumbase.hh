#ifndef systematic_smallenumbase_hh
#define systematic_smallenumbase_hh

#include <map>
#include <ostream>
#include <set>
#include <utility>
#include "forrest.hh"
#include "nodeindex.hh"

namespace systematic {

class INode;

class SmallEnumBase
{
    friend std::ostream &operator<<(std::ostream &os, const SmallEnumBase &se);
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
    SmallEnumBase();
    virtual ~SmallEnumBase();

    // not a constructor because it calls virtual functions
    void init(INode *root);

    TNodeIndex get_max_leaf() const
    {
        return leafCount;
    }

    TNodeIndex get_max_size(TNodeIndex n) const;

    TRange get_range(TNodeIndex n, TNodeIndex sz) const;

private:
    virtual void fill_leaf_map(INode *node, TLeafMap &map) = 0;

    virtual void add_one(const Forrest &forrest, TNodeIndex sz,
	const TLeafMap &map, TKnownSet &set) = 0;

    void add_left_subforrests(const Forrest &forrest,
	TNodeIndex sz, const TLeafMap &map, TKnownSet &set);

protected:
    void add_unknown_index(const Forrest &forrest, INode *leaf,
	TNodeIndex sz, const TLeafMap &map, TKnownSet &set);

    static void add_leaf(INode *node, TLeafMap &map);
};

std::ostream &operator<<(std::ostream &os, const SmallEnumBase &se);

}

#endif
