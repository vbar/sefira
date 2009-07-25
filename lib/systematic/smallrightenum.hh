#ifndef systematic_smallrightenum_hh
#define systematic_smallrightenum_hh

#include "nodeindex.hh"
#include "smallenumbase.hh"

namespace systematic {

class Forrest;
class INode;

class SmallRightEnum : public SmallEnumBase
{
private:
    virtual void fill_leaf_map(INode *node, TLeafMap &map);

    virtual void add_one(const Forrest &forrest, TNodeIndex sz,
	const TLeafMap &map, TKnownSet &set);
};

}

#endif
