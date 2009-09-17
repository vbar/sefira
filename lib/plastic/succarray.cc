#include "succarray.hh"
#include <stdexcept>

namespace plastic {

SuccArray::SuccArray(TNodeIndex array_size, TNodeIndex key_index):
    arraySize(array_size),
    keyIndex(key_index)
{
    if (!array_size)
    {
      throw std::invalid_argument("successor array must have size");
    }

    array = new TItem[array_size];
}

SuccArray::~SuccArray()
{
    delete [] array;
}

GraphEdge *SuccArray::pred(TNodeIndex idx, unsigned short key) const
{
    GraphEdge *ep = 0;
    // is the branch necessary?
    if (array[rebase(idx)].predecessor(key + 1, ep) != TItem::none)
    {
	return ep;
    }
    else
    {
	return 0;
    }
}

GraphEdge *SuccArray::succ(TNodeIndex idx, unsigned short key) const
{
    if (!key)
    {
        throw std::range_error("successor doesn't store 0");
    }

    GraphEdge *ep = 0;
    // is the branch necessary?
    if (array[rebase(idx)].successor(key - 1, ep) != TItem::none)
    {
	return ep;
    }
    else
    {
	return 0;
    }
}

void SuccArray::insert(TNodeIndex idx, const GraphEdge &e)
{
    array[rebase(idx)].insert(e.get_head().get_coord(keyIndex), e);
}

void SuccArray::erase(TNodeIndex idx, unsigned short key)
{
    array[rebase(idx)].erase(key);
}

TNodeIndex SuccArray::rebase(TNodeIndex idx) const
{
    if (!idx)
    {
        throw std::invalid_argument("SuccArray doesn't have index 0");
    }

    --idx;

    if (idx >= arraySize)
    {
        throw std::invalid_argument("SuccArray index too large");
    }

    return idx;
}

}
