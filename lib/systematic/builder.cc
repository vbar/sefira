#include "builder.hh"
#include "answer.hh"
#include "bigenum.hh"
#include "dynapad.hh"
#include "forrest.hh"
#include "inode.hh"
#include "smallenum.hh"
#include "smallenumbase.hh"
#include <assert.h>

#define NOTRACE
#include "trace.hh"

#define TRACE_overview true
#define TRACE_detail false

namespace systematic {

Builder::Builder(xmlNodePtr tree1, xmlNodePtr tree2):
    root1(tree1),
    root2(tree2)
{
    TRACE1("root1 = " << root1);
    TRACE1("root2 = " << root2);
}

Answer Builder::get_lcs()
{
    compute_lcs(&root1, &root2);

    bool swap = root1.get_size() < root2.get_size();
    Forrest small(swap ? &root1 : &root2);
    return fanPad.get(small);
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
        if (child == parent->get_right()) {
	    compute_period(f, g, parent, swap);
	} else {
	    compute_period_mirror(f, g, parent, swap);
	}

	child = parent;
	parent = parent->get_parent();
    }

    TRACE2(overview, "LCS = " << fanPad.get(swap ? f : g));
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
        bool swap)
{
    TRACE2(overview, "enter compute_period(" << f << ", " << g << ", " << parent << ", " << swap << ')');
    BigEnum fe(parent, false);
    short k = fe.get_size() - 1;
    assert(k > 0);
    Forrest fvp = fe.get(k);
    Forrest f0vp = fe.get(k - 1);
 
    SmallEnum ge(swap ? f : g, false);
    short i = ge.get_max_leaf();

    for (short ip = 1; ip <= i; ++ip) {
        short jip = ge.get_max_size(ip);
        DynaPad s;
	for (short kp = 1; kp <= k; ++kp) {
	    for (short jp = 1; jp <= jip; ++jp) {
	        SmallEnumBase::TRange rng = ge.get_range(ip, jp);
		for (SmallEnumBase::TConstIterator l = rng.first;
		     l != rng.second;
		     ++l) {
		    Forrest gipjp = l->second;
		    Answer a;
		    if (kp == 1) {
		        a = fanPad.get(gipjp);
			TRACE2(detail, "a(" << parent->get_right() << ", " << gipjp << ") = " << a);
			s.set(fe.get(kp - 1), gipjp, a);
		    } else {
		        a = s.get(fe.get(kp - 1), gipjp);
			TRACE2(detail, "a'(" << fe.get(kp - 1) << ", " << gipjp << ") = " << a);
		    }

		    Forrest fkp = fe.get(kp);
		    if (jp > 1) {
		        Forrest gprev(gipjp);
			gprev.pop_left_root();
			Answer b = s.get(fkp, gprev);
			TRACE2(detail, "b(" << fkp << ", " << gprev << ") = " << b);
			if (a.get_score() < b.get_score()) {
			    a = b;
			}
		    }

		    INode *v = fkp.get_left();
		    INode *w = gipjp.get_left();
		    TRACE2(detail, "v = " << v << ", w = " << w);
		    if (v->equals(w)) {
			Answer c;
			if (!v->is_leaf() && !w->is_leaf()) {
			    if (kp == k) {
			        Forrest fstem(v->get_left(), v->get_right());
				Forrest gstem(w->get_left(), w->get_right());
				const Answer *cp = s.try_get(fstem, gstem);
				if (cp) {
				    c = *cp;
				    TRACE2(detail, "c''(" << fstem << ", " << gstem << ") = " << c);
				} else {
				    c = fanPad.get(gstem);
				    TRACE2(detail, "c'(" << parent->get_right() << ", " << gstem << ") = " << c);
				}
			    } else { // v isn't on the heavy path
				c = swap ? stemTable.get(w, v) :
				    stemTable.get(v, w);
				if (swap) {
				  TRACE2(detail, "c(" << Forrest(w->get_left(), w->get_right()) << ", " << Forrest(v->get_left(), v->get_right()) << ") = " << c);
				} else {
				  TRACE2(detail, "c(" << Forrest(v->get_left(), v->get_right()) << ", " << Forrest(w->get_left(), w->get_right()) << ") = " << c);
				}
			    }
			}

			if ((v != fkp.get_right()) && (w != gipjp.get_right())) {
			    Forrest fold(fkp);
			    fold.pop_left_tree();
			    Forrest gold(gipjp);
			    gold.pop_left_tree();
			    Answer d = s.get(fold, gold);
			    c.insert(d);
			    TRACE2(detail, "c+d = " << c);
			}

			INode *first = swap ? w : v;
			c.insert(first->get_inner());
			TRACE2(detail, "final c = " << c);

			if (a.get_score() < c.get_score()) {
			    a = c;
			}
		    }

		    TRACE2(detail, "final a = " << a);
		    s.set(fkp, gipjp, a);
		}
	    }
	}

	for (short jp = 1; jp <= jip; ++jp) {
	    SmallEnumBase::TRange rng = ge.get_range(ip, jp);
	    for (SmallEnumBase::TConstIterator l = rng.first;
		 l != rng.second;
		 ++l) {
	        Forrest gipjp = l->second;
		fanPad.set(gipjp, s.get(fvp, gipjp));

		if (gipjp.is_tree()) {
		    Answer e;
		    if (!f0vp.is_empty()) {
		        Forrest gprev(gipjp);
			gprev.pop_left_root();
			if (!gprev.is_empty()) {
			    e = s.get(f0vp, gprev);
			}
		    }

		    if (swap) {
			stemTable.insert(gipjp.get_left(), parent, e);
		    } else {
			stemTable.insert(parent, gipjp.get_left(), e);
		    }
		}
	    }
	}
    }

    TRACE2(overview, "fanPad = " << fanPad);
}

void Builder::compute_period_mirror(INode *f, INode *g, INode *parent,
        bool swap)
{
    TRACE2(overview, "enter compute_period_mirror(" << f << ", " << g << ", " << parent << ", " << swap << ')');
    BigEnum fe(parent, true);
    short k = fe.get_size() - 1;
    assert(k > 0);
    Forrest fvp = fe.get(k);
    Forrest f0vp = fe.get(k - 1);

    SmallEnum ge(swap ? f : g, true);
    short i = ge.get_max_leaf();

    for (short ip = 1; ip <= i; ++ip) {
        short jip = ge.get_max_size(ip);
        DynaPad s;
	for (short kp = 1; kp <= k; ++kp) {
	    for (short jp = 1; jp <= jip; ++jp) {
	        SmallEnumBase::TRange rng = ge.get_range(ip, jp);
		assert(rng.first != rng.second);
		for (SmallEnumBase::TConstIterator l = rng.first;
		     l != rng.second;
		     ++l) {
		    Forrest gipjp = l->second;
		    Answer a;
		    if (kp == 1) {
		        a = fanPad.get(gipjp);
			TRACE2(detail, "a(" << parent->get_left() << ", " << gipjp << ") = " << a);
			s.set(fe.get(kp - 1), gipjp, a);

		    } else {
		        a = s.get(fe.get(kp - 1), gipjp);
			TRACE2(detail, "a'(" << fe.get(kp - 1) << ", " << gipjp << ") = " << a);
		    }

		    Forrest fkp = fe.get(kp);
		    if (jp > 1) {
		        Forrest gprev(gipjp);
			gprev.pop_right_root();
			Answer b = s.get(fkp, gprev);
			TRACE2(detail, "b(" << fkp << ", " << gprev << ") = " << b);
			if (a.get_score() < b.get_score()) {
			    a = b;
			}
		    }

		    INode *v = fkp.get_right();
		    INode *w = gipjp.get_right();
		    TRACE2(detail, "v = " << v << ", w = " << w);
		    if (v->equals(w)) {
			Answer c;
			if (!v->is_leaf() && !w->is_leaf()) {
			    if (kp == k) {
			        Forrest fstem(v->get_left(), v->get_right());
				Forrest gstem(w->get_left(), w->get_right());
				const Answer *cp = s.try_get(fstem, gstem);
				if (cp) {
				    c = *cp;
				    TRACE2(detail, "c''(" << fstem << ", " << gstem << ") = " << c);
				} else {
				    c = fanPad.get(gstem);
				    TRACE2(detail, "c'(" << parent->get_left() << ", " << gstem << ") = " << c);
				}
			    } else { // v isn't on the heavy path
				c = swap ? stemTable.get(w, v) :
				    stemTable.get(v, w);
				if (swap) {
				  TRACE2(detail, "c(" << Forrest(w->get_left(), w->get_right()) << ", " << Forrest(v->get_left(), v->get_right()) << ") = " << c);
				} else {
				  TRACE2(detail, "c(" << Forrest(v->get_left(), v->get_right()) << ", " << Forrest(w->get_left(), w->get_right()) << ") = " << c);
				}
			    }
			}

			if ((v != fkp.get_left()) && (w != gipjp.get_left())) {
			    Forrest fold(fkp);
			    fold.pop_right_tree();
			    Forrest gold(gipjp);
			    gold.pop_right_tree();
			    Answer d = s.get(fold, gold);
			    c.insert(d);
			    TRACE2(detail, "c+d = " << c);
			}

			INode *first = swap ? w : v;
			c.insert(first->get_inner());
			TRACE2(detail, "final c = " << c);

			if (a.get_score() < c.get_score()) {
			    a = c;
			}
		    }

		    TRACE2(detail, "final a = " << a);
		    s.set(fkp, gipjp, a);
		}
	    }
	}

	for (short jp = 1; jp <= jip; ++jp) {
	    SmallEnumBase::TRange rng = ge.get_range(ip, jp);
	    assert(rng.first != rng.second);
	    for (SmallEnumBase::TConstIterator l = rng.first;
		 l != rng.second;
		 ++l) {
	        Forrest gipjp = l->second;
		fanPad.set(gipjp, s.get(fvp, gipjp));

		if (gipjp.is_tree()) {
		    Answer e;
		    if (!f0vp.is_empty()) {
		        Forrest gprev(gipjp);
			gprev.pop_right_root();
			if (!gprev.is_empty()) {
			    e = s.get(f0vp, gprev);
			}
		    }

		    if (swap) {
			stemTable.insert(gipjp.get_right(), parent, e);
		    } else {
			stemTable.insert(parent, gipjp.get_right(), e);
		    }
		}
	    }
	}
    }

    TRACE2(overview, "fanPad = " << fanPad);
}

}
