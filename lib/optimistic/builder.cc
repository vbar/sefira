#include "builder.hh"
#include "answer.hh"
#include "relresult.hh"
#include "scoreboard.hh"
#include "shortmap.hh"
#include "srcgraph.hh"
#include <libxml/tree.h>
#include <iostream>
#include <assert.h>

#define NOTRACE
#include "trace.hh"

namespace optimistic {

typedef ShortMap<GraphEdge> TDestGraph;

Builder::Builder(xmlNodePtr tree1, xmlNodePtr tree2):
    dir1(tree1),
    dir2(tree2)
{
    TRACE1("dir1 = " << dir1);
    TRACE1("dir2 = " << dir2);
}

Answer Builder::get_lcs()
{
    GraphEdge d(
	TGraphPoint(1, 1),
	TGraphPoint(2 * dir1.get_node_count(),
		    2 * dir2.get_node_count()));
    RelResult r = compute_lcs(d);

    Answer a;
    for (RelResult::TSet::const_iterator i = r.begin();
	 i != r.end();
	 ++i)
    {
	xmlNodePtr n = dir1.recall(*i);
	assert(n);
	a.insert(n);
    }

    return a;
}

RelResult Builder::get_lcs(const GraphEdge &graph_edge)
{
    const RelResult *known = lcsMemo.get(graph_edge);
    if (known)
    {
	return *known;
    }

    TRACE1("before compute_lcs(" << graph_edge << ')');
    RelResult rv = compute_lcs(graph_edge);
    lcsMemo.insert(graph_edge, rv);
    TRACE1("compute_lcs(" << graph_edge << ") returned score " <<
	   rv.get_score());
    return rv;
}

RelResult Builder::compute_lcs(const GraphEdge &graph_edge)
{
    xmlNodePtr tree1 = dir1.recall(graph_edge.tail1());
    assert(tree1);
    xmlNodePtr tree2 = dir2.recall(graph_edge.tail2());
    assert(tree2);

    if (graph_edge.head1() - graph_edge.tail1() == 1)
    {
        RelResult result;
        if (dir2.find_first(tree1, graph_edge.tail2(), graph_edge.head2()))
	{
	    result.insert(graph_edge.tail1());
	}

	return result;
    } 
    else if (graph_edge.head2() - graph_edge.tail2() == 1)
    {
        RelResult result;
	TNodeIndex found = dir1.find_first(tree2, graph_edge.tail1(),
					   graph_edge.head1());
	if (found)
	{
	    TRACE1("inserting " << found);
	    result.insert(found);
	}

	return result;
    }

    SrcGraph srcGraph;
    for (Directory::TFullMap::const_iterator i = dir1.begin();
	 i != dir1.end();
	 ++i)
    {
        Directory::TAddressRange range = dir2.find(i->first,
            graph_edge.tail2());
	if (range.first != range.second)
	{
	    for (Directory::TAddress::const_iterator ii = 
		     Directory::lower_bound(i->second, graph_edge.tail1());
		 (ii != i->second.end()) &&
		     (ii->get_out() <= graph_edge.head1());
		 ++ii)
	    {
		for (Directory::TAddress::const_iterator jj = range.first;
		     (jj != range.second) &&
		         (jj->get_out() <= graph_edge.head2());
		     ++jj)
		{
		    if ((ii->get_in() > graph_edge.tail1()) ||
			(jj->get_in() > graph_edge.tail2()))
		    {
			GraphEdge e(
			    TGraphPoint(ii->get_in(), jj->get_in()),
			    TGraphPoint(ii->get_out(), jj->get_out()));
			srcGraph.insert(e);
		    }
		}	      
	    }
	}
    }

    equal_to_xmlNodePtr is_equal;
    TNodeIndex delta = is_equal(tree1, tree2) ? 0 : 1;

    assert((graph_edge.head1() - graph_edge.tail1() > 1) &&
	   (graph_edge.head2() - graph_edge.tail2() > 1));
    GraphEdge lastEdge(
	TGraphPoint(graph_edge.head1() - 1 + delta,
		    graph_edge.head2() - 1 + delta),
	TGraphPoint(graph_edge.head1() + delta,
		    graph_edge.head2() + delta));
    srcGraph.insert(lastEdge);

    TDestGraph succStruct;
    ScoreBoard score;
    for (TNodeIndex i = graph_edge.tail1() - delta;
	 i <= graph_edge.head1() + delta;
	 ++i)
    {
        TRACE1("i = " << i);
        const SrcGraph::TGroup &g = srcGraph.head_group(i);
	for (SrcGraph::TGroup::const_iterator ii = g.begin();
	     ii != g.end();
	     ++ii)
	{
	    TNodeIndex j = ii->head2();
	    TRACE1("j = " << j);

	    RelResult er = score.get(*ii);
	    TRACE1("cur = " << *ii << ", score = " << er.get_score());

	    GraphEdge *pe = 0;
	    unsigned int pred = succStruct.predecessor(j + 1, pe);
	    RelResult pr;
	    if (pred != TDestGraph::none)
	    {
	        TRACE1("pred = " << *pe << ", score = " << pr.get_score());
	        pr = score.get(*pe);
	    }

	    if (er.get_score() > pr.get_score())
	    {
		succStruct.set(j, *ii);
		unsigned int succ = succStruct.successor(j, pe);
		while ((succ != TDestGraph::none) &&
		       (score.get(*pe).get_score() <= er.get_score()))
		{
		    succStruct.erase(succ);
		    succ = succStruct.successor(j, pe);
		}
	    }
	}

        const SrcGraph::TGroup &h = srcGraph.tail_group(i);
	for (SrcGraph::TGroup::const_iterator ii = h.begin();
	     ii != h.end();
	     ++ii)
	{
	    TNodeIndex j = ii->tail2(); // missing in original pseudo-code

	    RelResult er;
	    if (*ii == lastEdge)
	    {
	        er.insert(graph_edge.tail1() - delta);
	    }
	    else
	    {
		if ((ii->head1() <= graph_edge.head1()) &&
		    (ii->head2() <= graph_edge.head2()))
		{
		    er = get_lcs(*ii);
		}
	    }

	    TRACE1("weight of " << *ii << " = " << er.get_score());
	    GraphEdge *pe = 0;
	    unsigned int pred = succStruct.predecessor(j + 1, pe);
	    if (pred != TDestGraph::none)
	    {
	        TRACE1("old pred = " << *pe << ", score = " << score.get(*pe));
	        er.insert(score.get(*pe));
	    }

	    TRACE1("new score of " << *ii << " = " << er.get_score());
	    score.insert(*ii, er);
	}
    }

    TRACE1("score = " << score);
    RelResult rv = score.get(lastEdge);
    TRACE1("raw answer = " << rv);
    return delta ? rv.crop(graph_edge.tail1()) : rv;
}

}
