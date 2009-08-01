#include "strprog.hh"
#include "answer.hh"
#include "straight/builder.hh"

Answer StrProg::get_lcs(xmlNodePtr tree1, xmlNodePtr tree2)
{
    straight::Builder builder(tree1, tree2);
    return builder.get_lcs();
}
