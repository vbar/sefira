#ifndef systematic_builder_hh
#define systematic_builder_hh

#include <libxml/tree.h>
#include "fanpad.hh"
#include "nodefactory.hh"
#include "rootnode.hh"
#include "smallenumcache.hh"
#include "stemtable.hh"

class Answer;

namespace systematic {

class INode;

class Builder
{
private:
    // used to initialize RootNode instances
    NodeFactory nodeFactory;

    RootNode root1;
    RootNode root2;
    StemTable stemTable;
    FanPad fanPad;

    // has shorter lifetime than both RootNode instances
    SmallEnumCache smallEnumCache;

    // initialized from RootNode instances
    const bool topSwap;

public:
    // trees passed to the constructor must stay valid for the
    // lifetime of the object
    Builder(xmlNodePtr tree1, xmlNodePtr tree2);

    bool is_swapped() const 
    {
        return topSwap;
    }

    Answer get_lcs();

private:    
    void compute_lcs(INode *f, INode *g);
    INode *do_top_light(INode *f, INode *g, bool swap);
    void compute_period(INode *f, INode *g, INode *parent,
			bool swap, bool twist);
};

}

#endif
