#ifndef systematic_smallenumcache_hh
#define systematic_smallenumcache_hh

#include <utility>

#include "smallenum.hh"

namespace systematic {

class INode;

class SmallEnumCache
{
private:
    typedef std::pair<INode *, SmallEnum> TEntry;

    TEntry cache[4];

public:
    SmallEnumCache();

    SmallEnum get_enum(INode *root, bool swap, bool twist);
};

}

#endif

