#include "builder.hh"
#include "answer.hh"
#include "hand.hh"
#include "postorder.hh"
#include <assert.h>

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
    assert(a);
    return *a;
}

}
