#include "builder.hh"
#include "answer.hh"
#include "bigenum.hh"
#include "dynapad.hh"
#include "forrest.hh"
#include "inode.hh"
#include "relresult.hh"
#include "smallenum.hh"
#include "smallenumbase.hh"
#include <assert.h>

#define NOTRACE
#include "trace.hh"

#define TRACE_overview true
#define TRACE_detail false

namespace systematic {

inline static INode *get_edge(const Forrest &forrest, bool twist)
{
    return twist ? forrest.get_back() : forrest.get_front();
}

// duplicates a Forrest constructor + a call to get_kernel, for the
// case when a Forrest instance isn't really necessary
static TKernel make_stem_kernel(INode *tip)
{
    INode *left = tip->get_left();
    INode *right = tip->get_right();
    if (!left) {
        left = right;
    } else if (!right) {
        right = left;
    }

    return TKernel(left->get_inorder(), right->get_inorder());
}

inline static TKernel make_node_kernel(INode *node)
{
    TNodeIndex inorder = node->get_inorder();
    return TKernel(inorder, inorder);
}

inline static TSubProblem make_sub_problem(INode *v, INode *w, bool swap)
{
    return swap ? TSubProblem(w->get_inorder(), v->get_inorder()) :
        TSubProblem(v->get_inorder(), w->get_inorder());
}

Builder::Builder(xmlNodePtr tree1, xmlNodePtr tree2):
    root1(tree1, nodeFactory),
    root2(tree2, nodeFactory)
{
    TRACE1("root1 = " << root1);
    TRACE1("root2 = " << root2);
}

Answer Builder::get_lcs()
{
    compute_lcs(&root1, &root2);

    bool swap = root1.get_size() < root2.get_size();
    RelResult r = fanPad.get(make_node_kernel(swap ? &root1 : &root2));

    Answer a;
    for (RelResult::TSet::const_iterator i = r.begin();
	 i != r.end();
	 ++i)
    {
	xmlNodePtr n = root1.get_at(*i);
	assert(n);
	a.insert(n);
    }

    return a;
}

void Builder::compute_lcs(INode *f, INode *g)
{
    TRACE2(overview, "enter compute_lcs(" << f << ", " << g << ')');
    bool swap = f->get_size() < g->get_size();
    INode *top = swap ? g : f;

    INode *parent = do_top_light(f, g, swap);

    fanPad.clear();
    TRACE2(overview, "fanPad reset");
    INode *child = 0;
    while (child != top) {
	compute_period(f, g, parent, swap, child == parent->get_left());
	child = parent;
	parent = parent->get_parent();
    }

    TRACE2(overview, "LCS = " << fanPad.get(make_node_kernel(swap ? f : g)));
}

INode *Builder::do_top_light(INode *f, INode *g, bool swap)
{
    TRACE2(overview, "enter do_top_light(" << f << ", " << g << ", " << swap << ')');
    INode *leaf = swap ? g : f;

    INode *left = leaf->get_left();
    INode *right = leaf->get_right();
    while (true) {
        if (left) {
	    if (right) {
		INode *heavy;
		INode *light;
		if (left->get_size() >= right->get_size()) {
		    heavy = left;
		    light = right;
		} else {
		    heavy = right;
		    light = left;
		}

		if (swap) {
		    compute_lcs(f, light);
		    leaf = do_top_light(f, heavy, swap);
		} else {
		    compute_lcs(light, g);
		    leaf = do_top_light(heavy, g, swap);
		}

		TRACE2(overview, "do_top_light returns " << leaf);
		return leaf;
	    } else {
	        leaf = left;
	        right = left->get_right();
		left = left->get_left();
	    }
	} else {
	    if (right) {
	        leaf = right;
	        left = right->get_left();
		right = right->get_right();
	    } else {
		TRACE2(overview, "do_top_light returns " << leaf);
	        return leaf;
	    }
	}
    }
}

void Builder::compute_period(INode *f, INode *g, INode *parent,
			     bool swap, bool twist)
{
    TRACE2(overview, "enter compute_period(" << f << ", " << g << ", " << parent << ", " << swap << ", " << twist << ')');
    BigEnum fe(parent, twist);
    TNodeIndex k = fe.get_size() - 1;
    assert(k > 0);
    Forrest fvp = fe.get(k);
    Forrest f0vp = fe.get(k - 1);
 
    SmallEnum ge = smallEnumCache.get_enum(swap ? f : g, swap, twist);
    TNodeIndex i = ge.get_max_leaf();

    for (TNodeIndex ip = 1; ip <= i; ++ip) {
        TNodeIndex jip = ge.get_max_size(ip);
        DynaPad s;
	for (TNodeIndex kp = 1; kp <= k; ++kp) {
	    for (TNodeIndex jp = 1; jp <= jip; ++jp) {
	        SmallEnumBase::TRange rng = ge.get_range(ip, jp);
		for (SmallEnumBase::TConstIterator l = rng.first;
		     l != rng.second;
		     ++l) {
		    Forrest gipjp = l->second;
		    RelResult a;
		    Forrest fprev = fe.get(kp - 1);
		    if (!fprev.is_empty()) {
		        TKernel gk = gipjp.get_kernel();
			if (kp == 1) {
			    a = fanPad.get(gk);
			    s.set(fprev.get_kernel(), gk, a);
			} else {
			    a = s.get(fprev.get_kernel(), gk);
			}
		    }

		    Forrest fkp = fe.get(kp);
		    if (jp > 1) {
		        Forrest gprev(gipjp);
			if (twist) {
			    gprev.pop_back_root();
			} else {
			    gprev.pop_front_root();
			}

			if (!gprev.is_empty()) {
			    RelResult b = s.get(
                                fkp.get_kernel(),
				gprev.get_kernel());
			    TRACE2(detail, "b(" << fkp << ", " << gprev << ") = " << b);
			    if (a.get_score() < b.get_score()) {
				a = b;
			    }
			}
		    }

		    INode *v = get_edge(fkp, twist);
		    INode *w = get_edge(gipjp, twist);
		    TRACE2(detail, "v = " << v << ", w = " << w);
		    if (v->equals(w)) {
			RelResult c;
			if (!v->is_leaf() && !w->is_leaf()) {
			    if (kp == k) {
			        TKernel gsk = make_stem_kernel(w);
				const RelResult *cp = s.try_get(
				    make_stem_kernel(v),
				    gsk);
				c = cp ? *cp : fanPad.get(gsk);
			    } else { // v isn't on the heavy path
			        c = stemTable.get(
                                    make_sub_problem(v, w, swap));
			    }
			}

			if (!fkp.is_tree() && !gipjp.is_tree()) {
			    Forrest fold(fkp);
			    Forrest gold(gipjp);
			    if (twist) {
			        fold.pop_back_tree();
				gold.pop_back_tree();
			    } else {
			        fold.pop_front_tree();
				gold.pop_front_tree();
			    }

			    RelResult d = s.get(fold.get_kernel(),
                                gold.get_kernel());
			    c.insert(d);
			    TRACE2(detail, "c+d = " << c);
			}

			INode *first = swap ? w : v;
			c.insert(first->get_inorder());
			TRACE2(detail, "final c = " << c);

			if (a.get_score() < c.get_score()) {
			    a = c;
			}
		    }

		    TRACE2(detail, "final a = " << a);
		    s.set(fkp.get_kernel(), gipjp.get_kernel(), a);
		}
	    }
	}

	for (TNodeIndex jp = 1; jp <= jip; ++jp) {
	    SmallEnumBase::TRange rng = ge.get_range(ip, jp);
	    for (SmallEnumBase::TConstIterator l = rng.first;
		 l != rng.second;
		 ++l) {
	        Forrest gipjp = l->second;
		TKernel gk = gipjp.get_kernel();
		fanPad.set(gk, s.get(fvp.get_kernel(), gk));

		if (gipjp.is_tree()) {
		    RelResult e;
		    if (!f0vp.is_empty()) {
		        Forrest gprev(gipjp);
			if (twist) {
			    gprev.pop_back_root();
			} else {
			    gprev.pop_front_root();
			}

			if (!gprev.is_empty()) {
			    e = s.get(f0vp.get_kernel(), gprev.get_kernel());
			}
		    }

		    stemTable.insert(
			make_sub_problem(parent, gipjp.get_front(), swap),
			e);
		}
	    }
	}
    }

    TRACE2(overview, "fanPad = " << fanPad);
}

}
