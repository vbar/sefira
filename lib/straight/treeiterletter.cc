#include "treeiterletter.hh"
#include "itreeanno.hh"
#include <stdexcept>

namespace straight
{

TreeIterLetter::TreeIterLetter(xmlNodePtr sentinel, const ITreeAnno &anno):
    sentinel(sentinel),
    anno(anno)
{
    current = sentinel ? sentinel->children : 0;
    skip_heavy();
}

void TreeIterLetter::inc()
{
    if (!current) {
	throw std::logic_error("iterator past end");
    }

    do_inc();
    skip_heavy();
}

void TreeIterLetter::do_inc()
{
    xmlNodePtr next = current->next;
    xmlNodePtr parent = current->parent;
    while (!next && (parent != sentinel)) {
        // parent is a heavy node
        next = parent->next;
	parent = parent->parent;
    }
    
    current = next;
}

void TreeIterLetter::skip_heavy()
{
    if (!current) {
        return;
    }

    while (!anno.is_light(current)) {
        xmlNodePtr ch = current->children;
	if (ch) {
	    current = ch;
	} else {
	    do_inc();
	    return;
	}
    }
}

}
