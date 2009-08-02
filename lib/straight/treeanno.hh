#ifndef treeanno_hh
#define treeanno_hh

#include <map>
#include <libxml/tree.h>
#include "nodeindex.hh"
#include "itreeanno.hh"
#include "nodeanno.hh"

namespace straight {

class TreeAnno : public ITreeAnno
{
private:
    typedef std::map<xmlNodePtr, NodeAnno> TAnno;

    xmlNodePtr docElem;

    TAnno anno;

public:
    // tree passed to the constructor must stay valid for the lifetime
    // of the object
    TreeAnno(xmlNodePtr doc_elem);

    xmlNodePtr get_tree() const { return docElem; }

    TNodeIndex get_desc_count(xmlNodePtr node) const;

    TNodeIndex get_preorder(xmlNodePtr node) const;

    virtual bool is_light(xmlNodePtr node) const;

private:
    NodeAnno *fill(xmlNodePtr node, TNodeIndex &preorder);

    TAnno::const_iterator get(xmlNodePtr node) const;
};

}

#endif
