#include "succarray.hh"
#include <stdexcept>

#define NOTRACE
#include "trace.hh"

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
        TRACE1("SuccArray: pred(" << idx << ", " << key << ") = " << *ep);
	return ep;
    }
    else
    {
        TRACE1("SuccArray: pred(" << idx << ", " << key << ") = 0");
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
        TRACE1("SuccArray: succ(" << idx << ", " << key << ") = " << *ep);
	return ep;
    }
    else
    {
        TRACE1("SuccArray: succ(" << idx << ", " << key << ") = 0");
	return 0;
    }
}

void SuccArray::set(TNodeIndex idx, const GraphEdge &e)
{
    TRACE1("SuccArray: " << idx << ", " << e.get_head().get_coord(keyIndex) << " := " << e);
    array[rebase(idx)].set(e.get_head().get_coord(keyIndex), e);
}

void SuccArray::erase(TNodeIndex idx, unsigned short key)
{
    TRACE1("SuccArray: erase " << idx << ", " << key);
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

void SuccArray::dump(std::ostream &os, TNodeIndex i) const
{
    TItem *current = array + i;
    if (current->is_empty())
    {
	os << "  { }";
    }
    else
    {
	os << "  {\n";
	std::string delim = "\t";
	GraphEdge *v = 0;
	unsigned int k = current->get_min(v);
	while (k != TItem::none)
	{
	    os << delim;
	    os << k << ": " << *v;
	    delim = ",\n\t";
	    k = current->successor(static_cast<unsigned short>(k), v);
	}

	os << "\n  }";
    }
}

std::ostream &operator<<(std::ostream &os, const SuccArray &sa)
{
    os << "[\n";
    std::string delim;
    for (TNodeIndex i = 0; i < sa.arraySize; ++i)
    {
        os << delim;
	delim = ",\n";
	sa.dump(os, i);
    }

    os << "\n]";
    return os;
}

}
