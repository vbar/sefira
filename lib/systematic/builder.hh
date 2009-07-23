#ifndef systematic_builder_hh
#define systematic_builder_hh

#include <libxml/tree.h>
#include "fanpad.hh"
#include "rootnode.hh"
#include "stemtable.hh"

class Answer;

namespace systematic {

class Forrest;
class INode;

class Builder
{
private:
    RootNode root1;
    RootNode root2;
    StemTable stemTable;
    FanPad fanPad;

public:
    // trees passed to the constructor must stay valid for the
    // lifetime of the object
    Builder(xmlNodePtr tree1, xmlNodePtr tree2);

    Answer get_lcs();

private:    
    void compute_lcs(INode *f, INode *g);
    INode *do_top_light(INode *f, INode *g, bool swap);
    void compute_period(INode *f, INode *g, INode *parent,
			bool swap);
    void compute_period_mirror(INode *f, INode *g, INode *parent,
			       bool swap);
};

}

#endif
