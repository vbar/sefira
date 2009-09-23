#include "builder.hh"
#include "answer.hh"
#include "relresult.hh"
#include "hand.hh"
#include "postorder.hh"

#define NOTRACE
#include "trace.hh"

namespace plastic {

Builder::Builder(xmlNodePtr tree1, xmlNodePtr tree2):
    tree1(tree1),
    tree2(tree2),
    decomposition(tree1, tree2),
    anno2(tree2)
{
    TRACE1("root1 = " << tree1);
    TRACE1("root2 = " << tree2);
}

Answer Builder::get_lcs()
{
    RelResult r = compute_lcs(tree1, tree2);

    Answer a;
    for (RelResult::TSet::const_iterator i = r.begin();
	 i != r.end();
	 ++i)
    {
	xmlNodePtr n = decomposition.get_at(*i);
	a.insert(n);
    }

    return a;
}

RelResult Builder::compute_lcs(xmlNodePtr f, xmlNodePtr g)
{
    TRACE1("enter compute_lcs(" << f << ", " << g << ')');
    PostOrder iter(f);
    PostOrder end;
    while (iter != end)
    {
	xmlNodePtr x = *iter;
	if (decomposition.get_leaf(x))
	{
	    Hand hand(x, g, &decomposition, anno2, &lcsMemo);
	    hand.compute();
	}

	++iter;
    }

    const RelResult *r = lcsMemo.get(f, g);
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
