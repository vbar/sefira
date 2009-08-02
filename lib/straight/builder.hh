#ifndef straight_builder_hh
#define straight_builder_hh

#include <libxml/tree.h>
#include "treeanno.hh"
#include "lcsmemo.hh"

class Answer;
class RelResult;

namespace straight {

class Forrest;

enum Direction { left, right };

class Builder
{
private:
    LcsMemo lcsMemo;
    TreeAnno anno1;
    TreeAnno anno2;

public:
    // trees passed to the constructor must stay valid for the
    // lifetime of the object
    Builder(xmlNodePtr tree1, xmlNodePtr tree2);

    Answer get_lcs();

private:    
    RelResult get_lcs(const Forrest &f, const Forrest &g, bool swap);

    RelResult do_get_lcs(xmlNodePtr node1, xmlNodePtr node2);

    RelResult compute_lcs(const Forrest &f, const Forrest &g, bool swap);

    Direction get_direction(const Forrest &f, const Forrest &g, bool swap);
};

}

#endif
