#include "annode.hh"
#include "compare.hh"
#include "inodefactory.hh"
#include "treetable.hh"
#include <stdexcept>
#include <assert.h>

namespace systematic {

AnNode::AnNode():
    size(1),
    inorder(0)
{
}

bool AnNode::is_leaf()
{
    return !get_left() && !get_right();
}

TNodeIndex AnNode::get_size()
{
    return size;
}

TNodeIndex AnNode::get_inorder()
{
    return inorder;
}

TNodeIndex AnNode::fill(TNodeIndex before, TreeTable &tt)
{
    if (inorder > 0) {
        throw std::logic_error("fill called twice");
    }

    INode *left = get_left();
    if (left) {
        inorder = left->fill(before, tt);
	assert(inorder > before);
	size += (inorder - before);
    } else {
        inorder = before;
    }

    tt.insert(get_inner());

    INode *right = get_right();
    TNodeIndex after;
    if (right) {
        after = right->fill(inorder + 1, tt);
	size += (after - inorder - 1);
    } else {
        after = inorder + 1;
    }

    return after;
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
