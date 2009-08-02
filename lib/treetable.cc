#include "treetable.hh"
#include <stdexcept>

TNodeIndex TreeTable::next() const
{
    TNodeIndex sz = static_cast<TNodeIndex>(table.size());
    if (sz != table.size()) {
        throw std::range_error("too many nodes");
    }

    return sz;
}
