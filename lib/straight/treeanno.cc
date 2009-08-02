#include "treeanno.hh"
#include <sstream>
#include <stdexcept>

namespace straight {

TreeAnno::TreeAnno(xmlNodePtr doc_elem):
    docElem(doc_elem)
{
}

TNodeIndex TreeAnno::get_desc_count(xmlNodePtr node) const
{
    TAnno::const_iterator i = get(node);
    return i->second.get_desc_count();
}

TNodeIndex TreeAnno::get_preorder(xmlNodePtr node) const
{
    TAnno::const_iterator i = get(node);
    return i->second.get_preorder();
}

bool TreeAnno::is_light(xmlNodePtr node) const
{
    TAnno::const_iterator i = get(node);
    return i->second.is_light();
}

NodeAnno *TreeAnno::fill(xmlNodePtr node, TreeTable &tt)
{
    TNodeIndex node_preorder = tt.next();
    tt.insert(node);

    TNodeIndex sum = 0;
    NodeAnno *max = 0;
    xmlNodePtr ch = node->children;
    while (ch) {
        NodeAnno *p = fill(ch, tt);
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
        TAnno::value_type(node, NodeAnno(sum, node_preorder)));
    if (!inres.second) {
        throw std::logic_error("repeated insert");
    }

    return &(inres.first->second);
}

TreeAnno::TAnno::const_iterator TreeAnno::get(xmlNodePtr node) const
{
    TAnno::const_iterator i = anno.find(node);
    if (i == anno.end()) {
        std::stringstream ss;
	ss << "node " << node << " not annotated";
        throw std::invalid_argument(ss.str());
    }

    return i;
}

}
