#include "doubleseq.hh"
#include "treeanno.hh"

namespace plastic {

DoubleSeq::DoubleSeq(xmlNodePtr g, const TreeAnno &anno):
    arraySize(2 * (1 + anno.get_desc_count(g)))
{
    array = new Item[arraySize];
    fill(g, 0);
}

DoubleSeq::~DoubleSeq()
{
    delete [] array;
}

TNodeIndex DoubleSeq::fill(xmlNodePtr node, TNodeIndex i)
{
    array[i].ystep = node;

    TNodeIndex j = i + 1;
    xmlNodePtr ch = node->children;
    while (ch)
    {
	j = fill(ch, j);
	ch = ch->next;
    }

    array[i].euler = j + 1;
    array[j].ystep = node;
    array[j].euler = i + 1;
    return j + 1;
}

}
