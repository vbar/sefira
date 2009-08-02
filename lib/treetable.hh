#ifndef treetable_hh
#define treetable_hh

#include <vector>
#include <libxml/tree.h>
#include "nodeindex.hh"

// inverse mapping from TNodeIndex to xmlNodePtr
class TreeTable
{
private:
    typedef std::vector<xmlNodePtr> TTable;

    TTable table;

public:
    xmlNodePtr get(TNodeIndex idx) const
    {
        return table.at(idx);
    }

    TNodeIndex next() const;

    void insert(xmlNodePtr inner)
    {
        table.push_back(inner);
    }
};

#endif
