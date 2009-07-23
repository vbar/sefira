#ifndef systematic_rootnode_hh
#define systematic_rootnode_hh

#include <libxml/tree.h>
#include "annode.hh"
#include "nodefactory.hh"

namespace systematic {

class INode;
class INodeFactory;

// presents the XML tree as binary
class RootNode : public AnNode
{
private:
    NodeFactory factory;
    xmlNodePtr inner;
    INode *left;
    INode *right;

public:
    RootNode(xmlNodePtr root);
    virtual ~RootNode();

    virtual bool equals(INode *other);

    virtual xmlNodePtr get_inner();

    virtual INode *get_parent();

    virtual INode *get_left();

    virtual INode *get_right();

    virtual void dump(std::ostream &os, int level) const;
};

}

#endif
