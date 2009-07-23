#ifndef systematic_smallleftenum_hh
#define systematic_smallleftenum_hh

#include <map>
#include <utility>
#include "nodeindex.hh"
#include "smallenum.hh"

namespace systematic {

class Forrest;
class INode;

class SmallLeftEnum : public SmallEnum
{
public:
    SmallLeftEnum(INode *root);

private:
    void fill_leaf_map(INode *node, TLeafMap &map);

    void add_left_subforrests(const Forrest &forrest, TNodeIndex sz,
			      const TLeafMap &map, TKnownSet &set);
    void add_one(const Forrest &forrest, TNodeIndex sz,
		 const TLeafMap &map, TKnownSet &set);
};

}

#endif
