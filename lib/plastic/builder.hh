#ifndef plastic_builder_hh
#define plastic_builder_hh

#include <libxml/tree.h>
#include "decomposition.hh"
#include "treescore.hh"
#include "treeanno.hh"

class Answer;
class RelResult;

namespace plastic {

class Graph;

class Builder
{
private:
    xmlNodePtr tree1;
    xmlNodePtr tree2;
    Decomposition decomposition;
    TreeAnno anno2;
    TreeScore lcsMemo;

public:
    // trees must stay valid for the lifetime of the constructed object
    Builder(xmlNodePtr tree1, xmlNodePtr tree2);

    Answer get_lcs();

private:
    RelResult get_lcs(xmlNodePtr f, xmlNodePtr g);
    RelResult compute_lcs(xmlNodePtr f, xmlNodePtr g);
};

}

#endif
