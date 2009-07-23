#include "smallleftenum.hh"
#include "inode.hh"
#include <stdexcept>
#include <utility>
#include <string.h>

namespace systematic {

SmallLeftEnum::SmallLeftEnum(INode *root)
{
    TLeafMap map;
    fill_leaf_map(root, map);
    leafCount = static_cast<TNodeIndex>(map.size());
    if (leafCount != map.size()) {
        throw std::range_error("SmallLeftEnum: too many leaves");
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
}

void SmallLeftEnum::fill_leaf_map(INode *node, TLeafMap &map)
{
    INode *left = node->get_left();
    INode *right = node->get_right();
    if (left) {
        fill_leaf_map(left, map);

	if (right) {
	    fill_leaf_map(right, map);
	}
    } else {
        if (right) {
	    fill_leaf_map(right, map);
	} else {
	    add_leaf(node, map);
	}
    }
}

void SmallLeftEnum::add_left_subforrests(const Forrest &forrest,
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

void SmallLeftEnum::add_one(const Forrest &forrest,
        TNodeIndex sz,
        const TLeafMap &map,
	TKnownSet &set)
{
    INode *leaf = forrest.get_left();
    INode *left = leaf->get_left();
    INode *right = leaf->get_right();
    while (left || right) {
        leaf = left ? left : right;
	left = leaf->get_left();
	right = leaf->get_right();
    }

    cond_add_index(forrest, leaf, sz, map, set);
}

}
