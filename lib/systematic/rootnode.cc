#include "rootnode.hh"
#include "inodefactory.hh"
#include "xutil.hh"
#include <assert.h>

namespace systematic {

RootNode::RootNode(xmlNodePtr root):
    inner(root),
    left(make_real_left(factory, root)),
    right(make_real_right(factory, root))
{
    assert(root);
}

RootNode::~RootNode()
{
    delete left;
    delete right;
}

bool RootNode::equals(INode *other)
{
    return is_equal(other);
}

xmlNodePtr RootNode::get_inner()
{
    return inner;
}

INode *RootNode::get_parent()
{
    return 0;
}

INode *RootNode::get_left()
{
    return left;
}

INode *RootNode::get_right()
{
    return right;
}

void RootNode::dump(std::ostream &os, int level) const
{
    start_line(os, level);
    os << xutil::get_node_name(inner) << " inside " << this
       << " (root)" << '\n';
    cond_dump(left, os, level + 1);
    os << '\n';
    cond_dump(right, os, level + 1);
}

}
