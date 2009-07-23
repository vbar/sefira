#include "sysprog.hh"
#include "answer.hh"
#include "systematic/builder.hh"

Answer SysProg::get_lcs(xmlNodePtr tree1, xmlNodePtr tree2)
{
    systematic::Builder builder(tree1, tree2);
    return builder.get_lcs();
}
