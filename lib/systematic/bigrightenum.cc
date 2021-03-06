#include "bigrightenum.hh"
#include "forrest.hh"
#include "inode.hh"

systematic::BigRightEnum::BigRightEnum(INode *parent)
{
    Forrest forrest(parent);
    INode *child = parent->get_left();
    emptyEdge = !child;
    if (child) {
        while (forrest.get_back() != child) {
	    seq.push_back(forrest);
	    forrest.pop_back_root();
	}

	seq.push_back(forrest);
    } else {
        while (!forrest.is_empty()) {
	    seq.push_back(forrest);
	    forrest.pop_back_root();
	}
    }
}
