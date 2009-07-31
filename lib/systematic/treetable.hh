#ifndef systematic_treetable_hh
#define systematic_treetable_hh

#include <vector>
#include <libxml/tree.h>
#include "nodeindex.hh"

namespace systematic {

// inverse mapping from TNodeIndex to xmlNodePtr
class TreeTable
{
private:
    typedef std::vector<xmlNodePtr> TTable;

    TTable table;

public:
    xmlNodePtr get(TNodeIndex idx)
    {
        return table.at(idx);
    }

    void insert(xmlNodePtr inner)
    {
        table.push_back(inner);
    }
};

}

#endif
