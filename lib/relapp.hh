#ifndef relapp_hh
#define relapp_hh

#include "answer.hh"
#include "program.hh"

// Program implementation for builders ordering trees by their size
template<typename TBuilder>
class RelApp : public Program
{
private:
    virtual Answer get_lcs(xmlNodePtr tree1, xmlNodePtr tree2, bool &second)
    {
	TBuilder builder(tree1, tree2);	
        second = !builder.is_swapped();
	return builder.get_lcs();
    }
};

#endif
