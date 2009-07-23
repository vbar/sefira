#include "inode.hh"
#include <assert.h>

namespace systematic {

INode::~INode()
{
}

std::ostream &operator<<(std::ostream &os, const INode &node)
{
    node.dump(os, 0);
    return os;
}

void start_line(std::ostream &os, int level)
{
    assert(level >= 0);
    while (level) {
        os << '\t';
	--level;
    }
}

void cond_dump(const INode *child, std::ostream &os, int level)
{
    if (child) {
        child->dump(os, level);
    } else {
        start_line(os, level);
        os << '0';
    }
}

}
