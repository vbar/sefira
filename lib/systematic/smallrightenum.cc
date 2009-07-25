#include "smallrightenum.hh"
#include "inode.hh"
#include <stdexcept>
#include <utility>
#include <string.h>

#define NOTRACE
#include "trace.hh"

namespace systematic {

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

void SmallRightEnum::add_one(const Forrest &forrest,
        TNodeIndex sz,
        const TLeafMap &map,
	TKnownSet &set)
{
    if (set.find(forrest) != set.end()) {
        return;
    }

    INode *leaf = forrest.get_back();
    INode *left = leaf->get_left();
    INode *right = leaf->get_right();
    while (left || right) {
	leaf = right ? right : left;
	left = leaf->get_left();
	right = leaf->get_right();
    }

    add_unknown_index(forrest, leaf, sz, map, set);
}

}
