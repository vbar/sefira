#include "optprog.hh"
#include "answer.hh"
#include "optimistic/builder.hh"

Answer OptProg::get_lcs(xmlNodePtr tree1, xmlNodePtr tree2)
{
    optimistic::Builder builder(tree1, tree2);
    return builder.get_lcs();
}
