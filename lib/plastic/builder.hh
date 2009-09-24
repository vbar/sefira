#ifndef plastic_builder_hh
#define plastic_builder_hh

#include <libxml/tree.h>
#include "decomposition.hh"
#include "doubleseq.hh"
#include "treescore.hh"

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
    DoubleSeq doubleSeq;
    TreeScore lcsMemo;

public:
    // trees must stay valid for the lifetime of the constructed object
    Builder(xmlNodePtr tree1, xmlNodePtr tree2);

    Answer get_lcs();

private:
    RelResult compute_lcs();
};

}

#endif
