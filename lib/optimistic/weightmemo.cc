#include "weightmemo.hh"

namespace optimistic {

const RelResult *WeightMemo::get(const GraphEdge &e) const
{
    TMap::const_iterator iter = memo.find(e.get_tail());
    return (iter == memo.end()) ? 0 : &(iter->second);
}

void WeightMemo::insert(const GraphEdge &e, RelResult r)
{
    memo.insert(TMap::value_type(e.get_tail(), r));
}

}
