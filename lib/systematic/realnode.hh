#ifndef systematic_realnode_hh
#define systematic_realnode_hh

#include <libxml/tree.h>
#include "annode.hh"

namespace systematic {

class INode;
class INodeFactory;

class RealNode : public AnNode
{
private:
    INode &parent;
    xmlNodePtr inner;
    INode *left;
    INode *right;

public:
    RealNode(INodeFactory &factory, INode &parent, xmlNodePtr inner);
    virtual ~RealNode();

    virtual bool equals(INode *other);

    virtual xmlNodePtr get_inner();

    virtual INode *get_parent();

    virtual INode *get_left();

    virtual INode *get_right();

    virtual void dump(std::ostream &os, int level) const;
};

}

#endif
