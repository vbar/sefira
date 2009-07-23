#ifndef systematic_inodefactory_hh
#define systematic_inodefactory_hh

#include <libxml/tree.h>

namespace systematic {

class INode;

class INodeFactory
{
public:
    virtual ~INodeFactory();

    virtual INode *create_backed(INode &parent, xmlNodePtr inner) = 0;

    virtual INode *create_ghost(INode &parent, xmlNodePtr left) = 0;  
};

}

#endif
