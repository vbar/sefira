#include "decomposition.hh"
#include "matchmap.hh"
#include <stdexcept>

namespace plastic {

Decomposition::Decomposition(xmlNodePtr tree1, xmlNodePtr tree2):
    anno(tree1)
{
    MatchMap mm(tree1, 1 + anno.get_desc_count(tree1), tree2);
    topSwap = !mm.is_size_ok();
    if (topSwap) {
        anno.reset(tree2);

	MatchMap altmm(tree2, 1 + anno.get_desc_count(tree2), tree1);
	if (!altmm.is_size_ok()) {
	    throw std::logic_error("unsatisfiable MatchMap");
	}

        otherTreeSize = altmm.get_other_tree_size();
	fill_path(tree2, tree2, altmm);

	otherTree = tree1;
    } else {
        otherTreeSize = mm.get_other_tree_size();
	fill_path(tree1, tree1, mm);

	otherTree = tree2;
    }
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
