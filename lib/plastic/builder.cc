#include "builder.hh"
#include "answer.hh"
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
}

Answer Builder::get_lcs()
{
    return compute_lcs(tree1, tree2);
}

Answer Builder::compute_lcs(xmlNodePtr f, xmlNodePtr g)
{
    TRACE1("enter compute_lcs(0x" << std::hex << f << ", 0x" << std::hex << g << ')');
    PostOrder iter(f);
    PostOrder end;
    while (iter != end)
    {
	xmlNodePtr x = *iter;
	if (decomposition.get_leaf(x))
	{
	    Hand hand(f, g, &decomposition, anno2, &lcsMemo);
	    hand.compute();
	}

	++iter;
    }

    const Answer *a = lcsMemo.get(f, g);
    if (a)
    {
        TRACE1("got score " << a->get_score());
	return *a;
    }
    else
    {
	TRACE1("LCS not memoized - assuming empty");
	return Answer();
    }
}

}
