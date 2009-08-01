#include "treeanno.hh"
#include <sstream>
#include <stdexcept>
#include <string>
#include <assert.h>

namespace straight {

TreeAnno::TreeAnno(xmlNodePtr doc_elem):
    docElem(doc_elem)
{
    fill(doc_elem);
}

TNodeIndex TreeAnno::get_desc_count(xmlNodePtr node) const
{
    TAnno::const_iterator i = anno.find(node);
    if (i == anno.end()) {
        throw std::invalid_argument("node not annotated");
    }

    return i->second.get_desc_count();
}

bool TreeAnno::is_light(xmlNodePtr node) const
{
    TAnno::const_iterator i = anno.find(node);
    if (i == anno.end()) {
        std::stringstream ss;
	ss << "node " << node << " not annotated";
        throw std::invalid_argument(ss.str());
    }

    return i->second.is_light();
}

NodeAnno *TreeAnno::fill(xmlNodePtr node)
{
    TNodeIndex sum = 0;
    NodeAnno *max = 0;
    xmlNodePtr ch = node->children;
    while (ch) {
        NodeAnno *p = fill(ch);
	TNodeIndex w = p->get_desc_count();
	sum += w + 1;
	if (!max || (w > max->get_desc_count())) {
	    max = p;
	}

	ch = ch->next;
    }

    if (max) {
        max->set_heavy();
    }

    std::pair<TAnno::iterator, bool> inres = anno.insert(
	TAnno::value_type(node, NodeAnno(sum)));
    assert(inres.second);
    return &(inres.first->second);
}

}
