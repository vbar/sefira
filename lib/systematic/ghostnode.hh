#ifndef systematic_ghostnode_hh
#define systematic_ghostnode_hh

#include <libxml/tree.h>
#include "annode.hh"

namespace systematic {

class INode;
class INodeFactory;

class GhostNode : public AnNode
{
private:
    INode &parent;
    INode *left;
    INode *right;

public:
    GhostNode(INodeFactory &factory, INode &parent, xmlNodePtr l);
    virtual ~GhostNode();

    virtual bool equals(INode *other);

    virtual xmlNodePtr get_inner();

    virtual INode *get_parent();

    virtual INode *get_left();

    virtual INode *get_right();

    virtual void dump(std::ostream &os, int level) const;

private:
    void init_left(INodeFactory &factory, xmlNodePtr l);
    void init_right(INodeFactory &factory, xmlNodePtr l);
};

}

#endif
