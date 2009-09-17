#ifndef plastic_forrestenum_hh
#define plastic_forrestenum_hh

#include <libxml/tree.h>
#include "nodeindex.hh"

namespace plastic {

class Decomposition;

class ForrestEnum
{
private:
    struct Item
    {
        xmlNodePtr xstep;
        bool left;
    };

    Item *array;
    const TNodeIndex arraySize;

public:
    ForrestEnum(xmlNodePtr f, const Decomposition &decomposition);
    ~ForrestEnum();

    bool is_left(TNodeIndex idx) const
    {
        return array[rebase(idx)].left;
    }

    xmlNodePtr get_xstep(TNodeIndex idx) const
    {
        return array[rebase(idx)].xstep;
    }

private:
    TNodeIndex rebase(TNodeIndex idx) const;

    // not implemented
    ForrestEnum(const ForrestEnum &other);
    ForrestEnum &operator=(const ForrestEnum &other);
};

}

#endif
