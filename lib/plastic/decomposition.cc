#include "decomposition.hh"
#include "matchmap.hh"
#include <stdexcept>

namespace plastic {

Decomposition::Decomposition(xmlNodePtr tree1, xmlNodePtr tree2):
    anno1(tree1)
{
    MatchMap mm(tree1, tree2);
    fill_path(tree1, tree1, mm);
}

xmlNodePtr Decomposition::get_leaf(xmlNodePtr path_root) const
{
    TCover::const_iterator i = cover.find(path_root);
    return (i == cover.end()) ? 0 : i->second;
}

void Decomposition::fill_path(xmlNodePtr first, xmlNodePtr current,
	const MatchMap &mm)
{
    xmlNodePtr kept = current->children;
    if (!kept) {
        insert(first, current);
    } else {
        TNodeIndex count = mm.get_count(kept);
	xmlNodePtr ch = kept->next;
	while (ch) {
	    TNodeIndex c = mm.get_count(ch);
	    if (c > count) {
	        fill_path(kept, kept, mm);
		kept = ch;
		count = c;
	    } else {
	        fill_path(ch, ch, mm);
	    }

	    ch = ch->next;
	}

	fill_path(first, kept, mm);
    }
}

void Decomposition::insert(xmlNodePtr first, xmlNodePtr last)
{
    std::pair<TCover::iterator, bool> inres = cover.insert(
        TCover::value_type(first, last));
    if (!inres.second) {
        throw std::logic_error("Decomposition: repeated insert");
    }
}

}
