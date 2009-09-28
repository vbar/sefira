#include "treeanno.hh"
#include <sstream>
#include <stdexcept>

namespace plastic {

TreeAnno::TreeAnno(xmlNodePtr doc_elem):
    docElem(doc_elem)
{
    fill(doc_elem);
}

void TreeAnno::reset(xmlNodePtr doc_elem)
{
    anno.clear();
    docElem = doc_elem;
    fill(doc_elem);
}

TNodeIndex TreeAnno::get_desc_count(xmlNodePtr node) const
{
    TAnno::const_iterator i = get(node);
    return i->second.get_desc_count();
}

NodeAnno *TreeAnno::fill(xmlNodePtr node)
{
    TNodeIndex sum = 0;
    xmlNodePtr ch = node->children;
    while (ch) {
        NodeAnno *p = fill(ch);
	TNodeIndex w = p->get_desc_count();
	sum += w + 1;

	ch = ch->next;
    }

    std::pair<TAnno::iterator, bool> inres = anno.insert(
        TAnno::value_type(node, NodeAnno(sum)));
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
