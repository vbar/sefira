#ifndef app_hh
#define app_hh

#include "answer.hh"
#include "program.hh"

// Program implementation for the usual builders
template<typename TBuilder>
class App : public Program
{
private:
    virtual Answer get_lcs(xmlNodePtr tree1, xmlNodePtr tree2)
    {
	TBuilder builder(tree1, tree2);
	return builder.get_lcs();
    }
};

#endif
