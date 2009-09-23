#ifndef plastic_decomposition_hh
#define plastic_decomposition_hh

#include <map>
#include <libxml/tree.h>
#include "nodeindex.hh"
#include "treeanno.hh"

namespace plastic {

class MatchMap;

class Decomposition
{
private:
    typedef std::map<xmlNodePtr, xmlNodePtr> TCover;

    // maps from first (root) node to the last (leaf) node of paths
    // covering the first tree passed to the constructor
    TCover cover;

    TreeAnno anno1;

public:
    // trees must stay valid for the lifetime of the constructed object
    Decomposition(xmlNodePtr tree1, xmlNodePtr tree2);

    xmlNodePtr get_at(TNodeIndex idx) const
    {
        return anno1.get_at(idx);
    }

    TNodeIndex get_preorder(xmlNodePtr node) const
    {
        return anno1.get_preorder(node);
    }

    // returns NULL when path_root isn't a root of any path
    xmlNodePtr get_leaf(xmlNodePtr path_root) const;

    TNodeIndex get_size(xmlNodePtr node) const
    {
        return 1 + anno1.get_desc_count(node);
    }

private:
    void fill_path(xmlNodePtr first, xmlNodePtr current,
	const MatchMap &mm);

    void insert(xmlNodePtr first, xmlNodePtr last);
};

}

#endif
