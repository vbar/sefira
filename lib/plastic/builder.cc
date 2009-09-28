#include "builder.hh"
#include "answer.hh"
#include "relresult.hh"
#include "hand.hh"
#include "postorder.hh"

#define NOTRACE
#include "trace.hh"

namespace plastic {

Builder::Builder(xmlNodePtr tree1, xmlNodePtr tree2):
    decomposition(tree1, tree2),
    doubleSeq(decomposition.get_other_tree(),
	      decomposition.get_other_tree_size())
{
    TRACE1("unswapped root1 = " << tree1);
    TRACE1("unswapped root2 = " << tree2);
}

Answer Builder::get_lcs()
{
    RelResult r = compute_lcs();

    Answer a;
    for (RelResult::TSet::const_iterator i = r.begin();
	 i != r.end();
	 ++i)
    {
        xmlNodePtr n = doubleSeq.get_ystep(*i);
	a.insert(n);
    }

    return a;
}

RelResult Builder::compute_lcs()
{
    TRACE1("enter compute_lcs()");
    xmlNodePtr tree1 = decomposition.get_tree();
    PostOrder iter(tree1);
    PostOrder end;
    while (iter != end)
    {
	xmlNodePtr x = *iter;
	if (decomposition.get_leaf(x))
	{
	    Hand hand(x, &decomposition, doubleSeq, &lcsMemo);
	    hand.compute();
	}

	++iter;
    }

    const RelResult *r = lcsMemo.get(tree1, doubleSeq.get_ystep(1));
    if (r)
    {
        TRACE1("got score " << r->get_score());
	return *r;
    }
    else
    {
	TRACE1("LCS not memoized - assuming empty");
	return RelResult();
    }
}

}
