#include "smallenumcache.hh"
#include <stdexcept>

namespace systematic {

SmallEnumCache::SmallEnumCache()
{
    for (size_t i = 0; i < 4; ++i)
    {
        cache[i].first = 0;
    }
}

SmallEnum SmallEnumCache::get_enum(INode *root, bool swap, bool twist)
{
    if (!root) {
        throw std::invalid_argument("root of valid small enum can't be null");
    }

    size_t idx = 2 * !!swap + !!twist;
    if (cache[idx].first != root)
    {
        cache[idx].second = SmallEnum(root, twist);
	cache[idx].first = root;
    }

    return cache[idx].second;
}

}
