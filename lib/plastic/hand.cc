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

namespace plastic {

Hand::Hand(xmlNodePtr f,
        const Decomposition *decomposition,
	const DoubleSeq &double_seq,
	TreeScore *master_score):
    decomposition(decomposition),
    n(decomposition->get_size(f)),
    mt2(2 * double_seq.get_tree_size()),
    forrestEnum(f, *decomposition),
    doubleSeq(double_seq),
    masterScore(master_score)
{
    TRACE1("enter Hand ctor(" << f << ", ...)");
    TRACE1("n = " << n << ", " << "2 * m = " << mt2);
    TRACE1("forrestEnum = " << forrestEnum);
    PathSet fmain(f, decomposition->get_leaf(f));
    TRACE1("main path = " << fmain);
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
        TRACE1("i = " << i);
        Graph::TRange trng = edgeGraph.get_tail_range(i - 1);
	for (Graph::TConstIterator iter = trng.first;
	     iter != trng.second;
	     ++iter)
	{
	    TRACE1("preprocessing " << iter->second);
	    const GraphPoint &tail = iter->second.get_tail();
  	    TNodeIndex j = tail.get_coord(
	        GraphPoint::small_first_index);
  	    TNodeIndex k = tail.get_coord(
	        GraphPoint::small_last_index);
	    RelResult a = get_score_cond(sright.pred(j, k));
	    RelResult b = get_score_cond(sleft.succ(k, j));
	    if (a.get_score() < b.get_score())
	    {
		a = b;
	    }

	    RelResult c = edgeGraph.get(iter->second);
	    TRACE1("w = " << c);
	    a.insert(c);
	    localScore.insert(iter->second, a);
	}

	bool is_right = !forrestEnum.is_left(i);
        Graph::TRange hrng = edgeGraph.get_head_range(i);
	for (Graph::TConstIterator iter = hrng.first;
	     iter != hrng.second;
	     ++iter)
	{
	    TRACE1("maximizing " << iter->second);
	    const GraphPoint &head = iter->second.get_head();
  	    TNodeIndex j = head.get_coord(
	        GraphPoint::small_first_index);
  	    TNodeIndex k = head.get_coord(
	        GraphPoint::small_last_index);
	    if (j > k - 1)
	    {
		throw std::logic_error("invalid edge head");
	    }

	    RelResult a = localScore.get(iter->second);
	    TNodeIndex s = a.get_score();
	    TRACE1("s = " << s);
	    if (is_right)
	    {
		RelResult b = get_score_cond(sright.pred(j, k));
		TRACE1("b = " << b.get_score());
		if (s > b.get_score())
		{
		    sright.set(j, iter->second);
		    GraphEdge *ep = sright.succ(j, k + 1);
		    while (ep && (get_score_sum(*ep) <= s))
		    {
			sright.erase(j, *ep);
			ep = sright.succ(j, k + 1);
		    }
		}

		GraphEdge *ep = sright.pred(j, k);
		if (ep)
		{
		    TRACE1("right: updating score of " << *ep);
		    GraphEdge edge(*ep);
		    RelResult c = localScore.get(edge);
		    GraphPoint &head = edge.get_head();
		    for (TNodeIndex ii = i; ii <= n; ++ii)
		    {
			head.set_coord(GraphPoint::big_index_index, ii);
			for (TNodeIndex kk = k; kk <= mt2; ++kk)
			{
			    head.set_coord(GraphPoint::small_last_index, kk);
			    update_score(edge, c);
			}
		    }
		}
	    }
	    else
	    {
		RelResult b = get_score_cond(sleft.succ(k, j));
		TRACE1("b = " << b.get_score());
		if (s > b.get_score())
		{
		    sleft.set(k, iter->second);
		    GraphEdge *ep = sleft.pred(k, j - 1);
		    while (ep && (get_score_sum(*ep) <= s))
		    {
		        sleft.erase(k, *ep);
			ep = sleft.pred(k, j - 1);
		    }
		}

		GraphEdge *ep = sleft.succ(k, j);
		if (ep)
		{
		    TRACE1("left: updating score of " << *ep);
		    GraphEdge edge(*ep);
		    RelResult c = localScore.get(edge);
		    GraphPoint &head = edge.get_head();
		    for (TNodeIndex ii = i; ii <= n; ++ii)
		    {
			head.set_coord(GraphPoint::big_index_index, ii);
			for (TNodeIndex jj = j; jj > 0; --jj)
			{
			    head.set_coord(GraphPoint::small_first_index, jj);
			    update_score(edge, c);
			}
		    }
		}
	    }
	}
    }

    TRACE1("sleft = " << sleft);
    TRACE1("sright = " << sright);

}

void Hand::cycle_left(TNodeIndex i, xmlNodePtr xi, bool on_main)
{
    TRACE1("enter cycle_left(" << i << ", " << xi << ", " << on_main << ')');
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
		        const RelResult *r = masterScore->get(xi, yj);
			if (!r)
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
			    *r);
		    }
		    else
		    {
			RelResult r;
			r.insert(j);
			edgeGraph.insert(
			    GraphEdge(
				GraphPoint(i - 1, j + 1, egj),
				GraphPoint(i, j, k)),
			    r);
		    }
		}
	    }
	}
    }
}

void Hand::cycle_right(TNodeIndex i, xmlNodePtr xi, bool on_main)
{
    TRACE1("enter cycle_right(" << i << ", " << xi << ", " << on_main << ')');
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
		    TRACE1(xi << " == " << yk);
		    if (!on_main)
		    {
		        const RelResult *r = masterScore->get(xi, yk);
			if (!r)
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
			    *r);
		    }
		    else
		    {
			RelResult r;
			r.insert(egk);
			edgeGraph.insert(
			    GraphEdge(
				GraphPoint(i - 1, egk, k - 1),
				GraphPoint(i, j, k)),
			    r);
		    }
		}
	    }
	}
    }
}

void Hand::update_score(const GraphEdge &e, const RelResult &r)
{
    TRACE1("enter update_score(" << e << ", " << r.get_score() << ')');
    xmlNodePtr x = forrestEnum.get_xtree(
        e.get_head().get_coord(GraphPoint::big_index_index));
    if (x)
    {
	xmlNodePtr y = doubleSeq.get_ystep(
	    e.get_head().get_coord(GraphPoint::small_first_index));
	assert(y);
	xmlNodePtr yp = doubleSeq.get_ystep(
	    e.get_head().get_coord(GraphPoint::small_last_index));
	assert(yp);
	if (y == yp)
	{
	    masterScore->update(x, y, r);
	}
	else
	{
	    TRACE1("small forrest isn't tree");
	}
    }
    else
    {
	TRACE1("big forrest isn't tree");
    }
}

}
