#ifndef systematic_nodefactory_hh
#define systematic_nodefactory_hh

#include "inodefactory.hh"
#include <libxml/tree.h>

namespace systematic {

class INode;

class NodeFactory : public INodeFactory
{
public:
    virtual INode *create_backed(INode &parent, xmlNodePtr inner);

    virtual INode *create_ghost(INode &parent, xmlNodePtr left);
};

}

#endif
