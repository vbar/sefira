#include "smallleftenum.hh"
#include "inode.hh"
#include <stdexcept>
#include <utility>
#include <string.h>

namespace systematic {

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

void SmallLeftEnum::add_one(const Forrest &forrest,
        TNodeIndex sz,
        const TLeafMap &map,
	TKnownSet &set)
{
    if (set.find(forrest) != set.end()) {
        return;
    }

    INode *leaf = forrest.get_left();
    INode *left = leaf->get_left();
    INode *right = leaf->get_right();
    while (left || right) {
        leaf = left ? left : right;
	left = leaf->get_left();
	right = leaf->get_right();
    }

    add_unknown_index(forrest, leaf, sz, map, set);
}

}
