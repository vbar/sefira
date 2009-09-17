#ifndef plastic_treeanno_hh
#define plastic_treeanno_hh

#include <map>
#include <libxml/tree.h>
#include "nodeindex.hh"
#include "nodeanno.hh"

namespace plastic {

class TreeAnno
{
private:
    typedef std::map<xmlNodePtr, NodeAnno> TAnno;

    TAnno anno;

public:
    // tree passed to the constructor must stay valid for the lifetime
    // of the object
    TreeAnno(xmlNodePtr doc_elem);

    TNodeIndex get_desc_count(xmlNodePtr node) const;

private:
    NodeAnno *fill(xmlNodePtr node);

    TAnno::const_iterator get(xmlNodePtr node) const;
};

}

#endif
