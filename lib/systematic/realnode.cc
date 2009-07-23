#include "realnode.hh"
#include "inodefactory.hh"
#include "xutil.hh"
#include <stdexcept>

namespace systematic {

RealNode::RealNode(INodeFactory &factory, INode &parent, xmlNodePtr inner):
    parent(parent),
    inner(inner),
    left(make_real_left(factory, inner)),
    right(make_real_right(factory, inner))
{
    if (!inner) {
        throw std::invalid_argument("no backing node");
    }
}

RealNode::~RealNode()
{
    delete left;
    delete right;
}

bool RealNode::equals(INode *other)
{
    return is_equal(other);
}

xmlNodePtr RealNode::get_inner()
{
    return inner;
}

INode *RealNode::get_parent()
{
    return &parent;
}

INode *RealNode::get_left()
{
    return left;
}

INode *RealNode::get_right()
{
    return right;
}

void RealNode::dump(std::ostream &os, int level) const
{
    start_line(os, level);
    os << xutil::get_node_name(inner) << " inside " << this << '\n';
    cond_dump(left, os, level + 1);
    os << '\n';
    cond_dump(right, os, level + 1);
}

}
