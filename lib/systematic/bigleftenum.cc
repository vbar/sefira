#include "bigleftenum.hh"
#include "inode.hh"
#include "forrest.hh"

systematic::BigLeftEnum::BigLeftEnum(INode *parent)
{
    Forrest forrest(parent);
    INode *child = parent->get_right();
    emptyEdge = !child;
    if (child) {
        while (forrest.get_front() != child) {
	    seq.push_back(forrest);
	    forrest.pop_front_root();
	}

	seq.push_back(forrest);
    } else {
        while (!forrest.is_empty()) {
	    seq.push_back(forrest);
	    forrest.pop_front_root();
	}
    }
}
