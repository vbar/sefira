#include "hand.hh"
#include "compare.hh"
#include "decomposition.hh"
#include "graphedge.hh"
#include "graphpoint.hh"
#include "pathset.hh"
#include "succarray.hh"
#include "treeanno.hh"
#include "treescore.hh"
#include <sstream>
#include <stdexcept>
#include <assert.h>

#define NOTRACE
#include "trace.hh"

#define TRACE_init true
#define TRACE_compute true

namespace plastic {

Hand::Hand(xmlNodePtr f, xmlNodePtr g,
        const Decomposition *decomposition, const TreeAnno &anno2,
	TreeScore *master_score):
    decomposition(decomposition),
    tree2(g),
    n(decomposition->get_size(f)),
    mt2(2 * (1 + anno2.get_desc_count(g))),
    forrestEnum(f, *decomposition),
    doubleSeq(g, anno2),
    masterScore(master_score)
{
    TRACE1("enter Hand ctor(" << f << ", " << g << ", ...)");
    TRACE2(init, "n = " << n << ", " << "2 * m = " << mt2);
    PathSet fmain(f, decomposition->get_leaf(f));
    TRACE2(init, "main path = " << fmain);
    for (TNodeIndex i = 1; i <= n; ++i)
    {
        xmlNodePtr xi = forrestEnum.get_xstep(i);
	bool on_main = fmain.is_on_path(xi);
        if (!forrestEnum.is_left(i))
	{
	    cycle_right(i, xi, on_main);
	}
	else
	{
	    cycle_left(i, xi, on_main);
	}
    }

    TRACE1("graph = " << edgeGraph);
}

void Hand::compute()
{
    TRACE1("enter Hand::compute");
    SuccArray sleft(mt2, GraphPoint::small_first_index);
    SuccArray sright(mt2, GraphPoint::small_last_index);
    for (TNodeIndex i = 1; i <= n; ++i)
    {
        TRACE2(compute, "i = " << i);
        Graph::TRange trng = edgeGraph.get_tail_range(i - 1);
	for (Graph::TConstIterator iter = trng.first;
	     iter != trng.second;
	     ++iter)
	{
	    TRACE2(compute, "preprocessing " << iter->second);
	    const GraphPoint &tail = iter->second.get_tail();
  	    TNodeIndex j = tail.get_coord(
	        GraphPoint::small_first_index);
  	    TNodeIndex k = tail.get_coord(
	        GraphPoint::small_last_index);
	    Answer a = get_score_cond(sright.pred(j, k));
	    Answer b = get_score_cond(sleft.succ(k, j));
	    if (a.get_score() < b.get_score())
	    {
		a = b;
	    }

	    Answer c = edgeGraph.get(iter->second);
	    TRACE2(compute, "w = " << c);
	    a.insert(c);
	    insert_score(iter->second, a);
	}

	bool is_right = !forrestEnum.is_left(i);
        Graph::TRange hrng = edgeGraph.get_head_range(i);
	for (Graph::TConstIterator iter = hrng.first;
	     iter != hrng.second;
	     ++iter)
	{
	    TRACE2(compute, "maximizing " << iter->second);
	    const GraphPoint &head = iter->second.get_head();
  	    TNodeIndex j = head.get_coord(
	        GraphPoint::small_first_index);
  	    TNodeIndex k = head.get_coord(
	        GraphPoint::small_last_index);
	    Answer a = get_score(iter->second);
	    TNodeIndex s = a.get_score();
	    TRACE2(compute, "s = " << s);
	    if (is_right)
	    {
		Answer b = get_score_cond(sright.pred(j, k));
		TRACE2(compute, "b = " << b.get_score());
		if (s > b.get_score())
		{
		    sright.insert(j, iter->second);
		    GraphEdge *ep = sright.succ(j, k + 1);
		    while (ep && (get_score_sum(*ep) <= s))
		    {
			sright.erase(j, *ep);
			ep = sright.succ(j, k + 1);
		    }
		}
	    }
	    else
	    {
		Answer b = get_score_cond(sleft.succ(k, j));
		TRACE2(compute, "b = " << b.get_score());
		if (s > b.get_score())
		{
		    sleft.insert(k, iter->second);
		    GraphEdge *ep = sleft.pred(k, j - 1);
		    while (ep && (get_score_sum(*ep) <= s))
		    {
		        sleft.erase(k, *ep);
			ep = sleft.pred(k, j - 1);
		    }
		}
	    }
	}

	if (is_right)
	{
	    Answer t = get_score_cond(sright.pred(1, mt2));
	    TRACE2(compute, "right total = " << t);
	    if (t.get_score() > 0)
	    {
	        masterScore->set(forrestEnum.get_xstep(i), tree2, t);
	    }
	}
	else
	{
	    Answer t = get_score_cond(sleft.succ(mt2, 1));
	    TRACE2(compute, "left total = " << t);
	    if (t.get_score() > 0)
	    {
		masterScore->set(forrestEnum.get_xstep(i), tree2, t);
	    }
	}
    }

    TRACE1("sleft = " << sleft);
    TRACE1("sright = " << sright);

}

void Hand::cycle_left(TNodeIndex i, xmlNodePtr xi, bool on_main)
{
    TRACE2(init, "enter cycle_left(" << i << ", " << xi << ", " << on_main << ')');
    equal_to_xmlNodePtr is_equal;

    for (TNodeIndex j = 1; j <= mt2; ++j)
    {
	TNodeIndex egj = doubleSeq.get_euler_index(j);
	if (j < egj)
	{
	    xmlNodePtr yj = doubleSeq.get_ystep(j);
	    if (is_equal(xi, yj))
	    {
		for (TNodeIndex k = egj; k <= mt2; ++k)
		{
		    if (!on_main)
		    {
		        const Answer *a = masterScore->get(xi, yj);
			if (!a)
			{
			    throw std::logic_error("expected distance not computed");
			}

			edgeGraph.insert(
			    GraphEdge(
				GraphPoint(
				    i - decomposition->get_size(xi),
				    egj,
				    k),
				GraphPoint(i, j, k)),
			    *a);
		    }
		    else
		    {
			Answer a;
			a.insert(xi);
			masterScore->set(xi, yj, a);
			edgeGraph.insert(
			    GraphEdge(
				GraphPoint(i - 1, j + 1, egj),
				GraphPoint(i, j, k)),
			    a);
		    }
		}
	    }
	}
    }
}

void Hand::cycle_right(TNodeIndex i, xmlNodePtr xi, bool on_main)
{
    TRACE2(init, "enter cycle_right(" << i << ", " << xi << ", " << on_main << ')');
    equal_to_xmlNodePtr is_equal;

    for (TNodeIndex j = 1; j <= mt2; ++j)
    {
	for (TNodeIndex k = j + 1; k <= mt2; ++k)
	{
	    TNodeIndex egk = doubleSeq.get_euler_index(k);
	    if ((j <= egk) && (egk < k))
	    {
		xmlNodePtr yk = doubleSeq.get_ystep(k);
		if (is_equal(xi, yk))
		{
		    TRACE2(init, xi << " == " << yk);
		    if (!on_main)
		    {
		        const Answer *a = masterScore->get(xi, yk);
			if (!a)
			{
			    throw std::logic_error("expected distance not computed");
			}

			edgeGraph.insert(
			    GraphEdge(
				GraphPoint(
				      i - decomposition->get_size(xi),
				      j,
				      egk),
				GraphPoint(i, j, k)),
			    *a);
		    }
		    else
		    {
			Answer a;
			a.insert(xi);
			masterScore->set(xi, yk, a);
			edgeGraph.insert(
			    GraphEdge(
				GraphPoint(i - 1, egk, k - 1),
				GraphPoint(i, j, k)),
			    a);
		    }
		}
	    }
	}
    }
}

Answer Hand::get_score(const GraphEdge &e) const
{
    xmlNodePtr y = doubleSeq.get_ystep(
        e.get_head().get_coord(GraphPoint::small_first_index));
    assert(y);
    xmlNodePtr yp = doubleSeq.get_ystep(
        e.get_head().get_coord(GraphPoint::small_last_index));
    assert(yp);

    if (y == yp)
    {
        xmlNodePtr x = forrestEnum.get_xstep(
	    e.get_head().get_coord(GraphPoint::big_index_index));
	const Answer *a = masterScore->get(x, y);
	if (!a)
	{
	    throw std::logic_error("expected distance not in master score");
	}

	return *a;
    }
    else
    {
	const Answer *a = localScore.get(e);
	if (a)
	{
	    return *a;
	}
	else
	{
	    TRACE2(compute, "using weight of " << e << " as score");
	    return edgeGraph.get(e);
	}
    }
}

void Hand::insert_score(const GraphEdge &e, const Answer &a)
{
    TRACE2(compute, "enter insert_score(" << e << ", " << a.get_score() << ')');
    xmlNodePtr y = doubleSeq.get_ystep(
        e.get_head().get_coord(GraphPoint::small_first_index));
    assert(y);
    xmlNodePtr yp = doubleSeq.get_ystep(
        e.get_head().get_coord(GraphPoint::small_last_index));
    assert(yp);

    if (y == yp)
    {
        xmlNodePtr x = forrestEnum.get_xstep(
            e.get_head().get_coord(GraphPoint::big_index_index));
	masterScore->set(x, y, a);
	TRACE2(compute, "to master");
    }
    else
    {
	localScore.insert(e, a);
	TRACE2(compute, "to local");
    }
}

}
