#ifndef systematic_smallrightenum_hh
#define systematic_smallrightenum_hh

#include <map>
#include <utility>
#include "nodeindex.hh"
#include "smallenum.hh"

namespace systematic {

class Forrest;
class INode;

class SmallRightEnum : public SmallEnum
{
public:
    SmallRightEnum(INode *root);

private:
    void fill_leaf_map(INode *node, TLeafMap &map);

    void add_left_subforrests(const Forrest &forrest, TNodeIndex sz,
			      const TLeafMap &map, TKnownSet &set);
    void add_one(const Forrest &forrest, TNodeIndex sz,
		 const TLeafMap &map, TKnownSet &set);
};

}

#endif
