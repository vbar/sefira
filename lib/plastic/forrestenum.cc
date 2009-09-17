#include "forrestenum.hh"
#include "decomposition.hh"
#include "forrest.hh"
#include "pathset.hh"
#include <set>
#include <stdexcept>

namespace plastic {

ForrestEnum::ForrestEnum(xmlNodePtr f, const Decomposition &decomposition):
    arraySize(decomposition.get_size(f))
{
    PathSet path(f, decomposition.get_leaf(f));
    
    array = new Item[arraySize];
    Forrest forrest;
    forrest.insert(f);
    for (TNodeIndex i = arraySize - 1; i > 0; --i)
    {
	xmlNodePtr left = forrest.get_front();
	if (!path.is_on_path(left))
	{
	    array[i].xstep = forrest.pop_front_root();
	    array[i].left = true;
	}
	else
	{
	    array[i].xstep = forrest.pop_back_root();
	    array[i].left = false;
	}
    }

    array[0].xstep = forrest.get_back(); // there's only one
    array[0].left = false;
}

ForrestEnum::~ForrestEnum()
{
    delete [] array;
}

TNodeIndex ForrestEnum::rebase(TNodeIndex idx) const
{
    if (!idx)
    {
        throw std::invalid_argument("ForrestEnum doesn't have index 0");
    }

    --idx;

    if (idx >= arraySize)
    {
        throw std::invalid_argument("ForrestEnum index too large");
    }

    return idx;
}

}
