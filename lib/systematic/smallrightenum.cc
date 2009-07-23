#include "smallrightenum.hh"
#include "inode.hh"
#include <stdexcept>
#include <utility>
#include <string.h>

#define NOTRACE
#include "trace.hh"

namespace systematic {

SmallRightEnum::SmallRightEnum(INode *root)
{
    TRACE1("enter SmallRightEnum");
    TLeafMap map;
    fill_leaf_map(root, map);
    leafCount = static_cast<TNodeIndex>(map.size());
    if (leafCount != map.size()) {
        throw std::range_error("SmallRightEnum: too many leaves");
    }

    TKnownSet set;

    maxSize = new TNodeIndex[leafCount];
    memset(maxSize, 0, sizeof(TNodeIndex) * leafCount);

    // inefficient - tries to add some subforrests twice
    Forrest f(root);
    TNodeIndex sz = root->get_size();
    while (sz) {
        add_left_subforrests(f, sz, map, set);
	f.pop_right_root();
	--sz;
    }
    TRACE1("exit SmallRightEnum");
}

void SmallRightEnum::fill_leaf_map(INode *node, TLeafMap &map)
{
    INode *left = node->get_left();
    INode *right = node->get_right();
    if (left) {
	if (right) {
	    fill_leaf_map(right, map);
	}

        fill_leaf_map(left, map);
    } else {
        if (right) {
	    fill_leaf_map(right, map);
	} else {
	    add_leaf(node, map);
	}
    }
}

void SmallRightEnum::add_left_subforrests(const Forrest &forrest,
        TNodeIndex sz,
        const TLeafMap &map,
        TKnownSet &set)
{
    Forrest f(forrest);
    while (sz) {
        add_one(f, sz, map, set);
	f.pop_left_root();
	--sz;
    }
}

void SmallRightEnum::add_one(const Forrest &forrest,
        TNodeIndex sz,
        const TLeafMap &map,
	TKnownSet &set)
{
    TRACE1("enter add_one(" << forrest << ", " << sz << "... )");
    INode *leaf = forrest.get_right();
    INode *left = leaf->get_left();
    INode *right = leaf->get_right();
    while (left || right) {
	leaf = right ? right : left;
	left = leaf->get_left();
	right = leaf->get_right();
    }

    cond_add_index(forrest, leaf, sz, map, set);
    TRACE1("exit add_one");
}

}
