#include "doubleseq.hh"
#include "treeanno.hh"
#include <string.h>

namespace plastic {

DoubleSeq::DoubleSeq(xmlNodePtr g, TNodeIndex sz):
    treeSize(sz)
{
    array = new Item[2 * treeSize];
    fill(g, 0);
}

DoubleSeq::DoubleSeq(const DoubleSeq &other):
    treeSize(other.treeSize)
{
    array = new Item[2 * treeSize];
    memcpy(array, other.array, 2 * treeSize * sizeof(Item));
}

DoubleSeq &DoubleSeq::operator=(const DoubleSeq &other)
{
    treeSize = other.treeSize;
    Item *a = new Item[2 * treeSize]; 
    memcpy(a, other.array, 2 * treeSize * sizeof(Item));
    delete [] array;
    array = a;
    return *this;
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
