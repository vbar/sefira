#include "builder.hh"
#include "answer.hh"
#include "compare.hh"
#include "forrest.hh"
#include "treeiter.hh"
#include <iomanip>
#include <iostream>

#define NOTRACE
#include "trace.hh"

namespace straight {

static Forrest make_root_forrest(xmlNodePtr tree)
{
    Forrest f;
    f.insert(tree);
    return f;
}

static Forrest make_child_forrest(xmlNodePtr tree, const TreeAnno &anno)
{
    Forrest f;
    xmlNodePtr ch = tree->children;
    while (ch) {
        f.insert(ch);
	ch = ch->next;
    }

    return f;
}

Builder::Builder(xmlNodePtr tree1, xmlNodePtr tree2):
    anno1(tree1), anno2(tree2)
{
    TRACE1("Builder ctor(" << tree1 << ", " << tree2 << ')');
}

Answer Builder::get_lcs()
{
    return do_get_lcs(anno1.get_tree(), anno2.get_tree());
}

Answer Builder::get_lcs(const Forrest &f, const Forrest &g, bool swap)
{
    TRACE1("enter get_lcs(" << f << ", " << g << ')');
    if (f.is_empty() || g.is_empty()) {
        return Answer();
    }

    const Answer *a = lcsMemo.get(f, g);
    if (a) {
        return *a;
    } else {
        Answer b = (f.is_tree() && g.is_tree()) ? 
	    do_get_lcs(f.get_front(), g.get_front()) :
	    compute_lcs(f, g, swap);
	lcsMemo.insert(f, g, b);
	return b;
    }
}

Answer Builder::do_get_lcs(xmlNodePtr node1, xmlNodePtr node2)
{
    TRACE1("enter do_get_lcs(" << node1 << ", " << node2 << ')');
    Forrest f(make_root_forrest(node1));
    Forrest g(make_root_forrest(node2));
    bool swap = anno1.get_desc_count(node1) < anno2.get_desc_count(node2);
    TRACE1("got desc count");
    TreeIter iter = !swap ? TreeIter(node1, anno1) : TreeIter(node2, anno2);
    TreeIter end(!swap ? anno1 : anno2);
    for (; iter != end; ++iter) {
        if (!swap) {
	    get_lcs(make_root_forrest(*iter), g, swap);
	} else {
	    get_lcs(f, make_root_forrest(*iter), swap);
	}
    }

    return compute_lcs(f, g, swap);
}

Answer Builder::compute_lcs(const Forrest &f, const Forrest &g, bool swap)
{
    xmlNodePtr rf, rg;
    Forrest fnl(f);
    Forrest gnl(g);
    Direction dir = get_direction(f, g, swap);
    if (dir == right) {
        rf = fnl.pop_back_root();
	rg = gnl.pop_back_root();
    } else {
        rf = fnl.pop_front_root();
	rg = gnl.pop_front_root();
    }

    TRACE1("1: " << fnl << ", " << g);
    Answer a1 = get_lcs(fnl, g, swap);
    TRACE1("2: " << f << ", " << gnl);
    Answer a2 = get_lcs(f, gnl, swap);

    if (!compare(rf, rg, false)) {
        Forrest ftl(f);
        Forrest gtl(g);
	if (dir == right) {
	    ftl.pop_back_tree();
	    gtl.pop_back_tree();
	} else {
	    ftl.pop_front_tree();
	    gtl.pop_front_tree();
	}

	Forrest chf(make_child_forrest(rf, anno1));
	Forrest chg(make_child_forrest(rg, anno2));
	TRACE1("3: " << chf << ", " << chg);
	Answer a = get_lcs(chf, chg, swap);

	TRACE1("4: " << ftl << ", " << gtl);
	Answer rest = get_lcs(ftl, gtl, swap);
	a.insert(rest);
	a.insert(rf);

	if (a.get_score() >= a1.get_score()) {
	    a1 = a;
	}
    }

    TRACE1("got answer");
    return (a1.get_score() >= a2.get_score()) ? a1 : a2;
}

Direction Builder::get_direction(const Forrest &f, const Forrest &g, bool swap)
{
    const Forrest *pf = !swap ? &f : &g;
    const TreeAnno *anno = !swap ? &anno1 : &anno2;
    return (pf->is_tree() || anno->is_light(pf->get_front())) ? left : right;
#if 0
    // Zhang-Shasha
    return right;
#endif
#if 0
    // Klein
    bool swap = anno1.get_desc_count(tree1) < anno2.get_desc_count(tree2);
    const TreeAnno *anno;
    const Forrest *pf;
    if (!swap) {
        anno = &anno1;
	pf = &f;
    } else {
        anno = &anno2;
	pf = &g;
    }

    return (anno->get_desc_count(pf->get_front()) <= 
	    anno->get_desc_count(pf->get_back())) ? left : right;
#endif
}

}
