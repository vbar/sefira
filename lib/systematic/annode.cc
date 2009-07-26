#include "annode.hh"
#include "compare.hh"
#include "inodefactory.hh"
#include <stdexcept>

namespace systematic {

AnNode::AnNode():
    size(0)
{
}

bool AnNode::is_leaf()
{
    return !get_left() && !get_right();
}

TNodeIndex AnNode::get_size()
{
    if (!size) {
	int sz = 1;

	INode *left = get_left();
	if (left) {
	    sz += left->get_size();
	}

	INode *right = get_right();
	if (right) {
	    sz += right->get_size();
	}

	size = sz;
	if (size != sz) {
	    throw std::range_error("tree too large");
	}
    }

    return size;
}

TNodeIndex AnNode::get_inorder()
{
    return inorder;
}

TNodeIndex AnNode::fill(TNodeIndex before)
{
    INode *left = get_left();
    if (left) {
        inorder = left->fill(before);
    } else {
        inorder = before;
    }

    INode *right = get_right();
    if (right) {
        return right->fill(inorder + 1);
    } else {
        return inorder + 1;
    }
}

INode *AnNode::make_real_left(INodeFactory &factory, xmlNodePtr inner)
{
    xmlNodePtr ch = inner->children;
    return ch ? factory.create_backed(*this, ch) : 0;
}

INode *AnNode::make_real_right(INodeFactory &factory, xmlNodePtr inner)
{
    INode *right = 0;
    xmlNodePtr ch = inner->children;
    if (ch) {
	xmlNodePtr p = ch->next;
	if (p) {
	    xmlNodePtr q = p->next;
	    if (q) {
		right = factory.create_ghost(*this, p);
	    } else {
		right = factory.create_backed(*this, p);
	    }
	}
    }

    return right;
}

bool AnNode::is_equal(INode *other)
{
    xmlNodePtr inner = get_inner();
    if (!inner) {
        throw std::logic_error("only real nodes can be equal");
    }

    xmlNodePtr other_inner = other->get_inner();
    if (!other_inner) { // isn't the check already inside compare?
        return false;
    }

    return !compare(inner, other_inner, false);
}

}
