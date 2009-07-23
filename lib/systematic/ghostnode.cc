#include "ghostnode.hh"
#include "inodefactory.hh"
#include <string>
#include <assert.h>

namespace systematic {

GhostNode::GhostNode(INodeFactory &factory, INode &parent, xmlNodePtr l):
    parent(parent)
{
    assert(l);
    init_left(factory, l);
    init_right(factory, l);
}

GhostNode::~GhostNode()
{
    delete left;
    delete right;
}

bool GhostNode::equals(INode *other)
{
    return false;
}

xmlNodePtr GhostNode::get_inner()
{
    return 0;
}

INode *GhostNode::get_parent()
{
    return &parent;
}

INode *GhostNode::get_left()
{
    return left;
}

INode *GhostNode::get_right()
{
    return right;
}

void GhostNode::init_left(INodeFactory &factory, xmlNodePtr l)
{
    left = factory.create_backed(*this, l);
}

void GhostNode::init_right(INodeFactory &factory, xmlNodePtr l)
{
    xmlNodePtr p = l->next;
    assert(p);
    xmlNodePtr q = p->next;
    if (q) {
        right = factory.create_ghost(*this, p);
    } else {
        right = factory.create_backed(*this, p);
    }
}

void GhostNode::dump(std::ostream &os, int level) const
{
    start_line(os, level);
    os << "<ghost> at " << this << '\n';
    cond_dump(left, os, level + 1);
    os << '\n';
    cond_dump(right, os, level + 1);
}

}
