#ifndef straight_treeanno_hh
#define straight_treeanno_hh

#include <map>
#include <libxml/tree.h>
#include "nodeindex.hh"
#include "itreeanno.hh"
#include "nodeanno.hh"
#include "treetable.hh"

namespace straight {

class TreeAnno : public ITreeAnno
{
private:
    typedef std::map<xmlNodePtr, NodeAnno> TAnno;

    xmlNodePtr docElem;
    TreeTable treeTable;
    TAnno anno;

public:
    // tree passed to the constructor must stay valid for the lifetime
    // of the object
    TreeAnno(xmlNodePtr doc_elem);

    xmlNodePtr get_tree() const { return docElem; }

    xmlNodePtr get_at(TNodeIndex idx) const
    {
        return treeTable.get(idx);
    }

    TNodeIndex get_desc_count(xmlNodePtr node) const;

    TNodeIndex get_preorder(xmlNodePtr node) const;

    virtual bool is_light(xmlNodePtr node) const;

    void fill()
    {
        fill(docElem);
    }

private:
    NodeAnno *fill(xmlNodePtr node);

    TAnno::const_iterator get(xmlNodePtr node) const;
};

}

#endif
