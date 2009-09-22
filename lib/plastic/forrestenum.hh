#ifndef plastic_forrestenum_hh
#define plastic_forrestenum_hh

#include <iostream>
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
    friend std::ostream &operator<<(std::ostream &os, const ForrestEnum &fe);

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

    void dump(std::ostream &os, TNodeIndex i) const;

    // not implemented
    ForrestEnum(const ForrestEnum &other);
    ForrestEnum &operator=(const ForrestEnum &other);
};

std::ostream &operator<<(std::ostream &os, const ForrestEnum &fe);

}

#endif
