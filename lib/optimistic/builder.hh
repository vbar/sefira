#ifndef optimistic_builder_hh
#define optimistic_builder_hh

#include "directory.hh"
#include "weightmemo.hh"

class Answer;
class RelResult;

namespace optimistic {

class GraphEdge;

class Builder
{
private:
    WeightMemo lcsMemo;
    Directory dir1;
    Directory dir2;

public:
    // trees passed to the constructor must stay valid for the
    // lifetime of the object
    Builder(xmlNodePtr tree1, xmlNodePtr tree2);

    Answer get_lcs();

private:
    RelResult get_lcs(const GraphEdge &graph_edge);

    RelResult compute_lcs(const GraphEdge &graph_edge);
};

}

#endif
