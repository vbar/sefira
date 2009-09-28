#ifndef systematic_rootnode_hh
#define systematic_rootnode_hh

#include <libxml/tree.h>
#include "annode.hh"
#include "treetable.hh"

namespace systematic {

class INode;
class NodeFactory;

// presents the XML tree as binary
class RootNode : public AnNode
{
private:
    TreeTable treeTable;
    xmlNodePtr inner;
    INode *left;
    INode *right;

public:
    RootNode(xmlNodePtr root, NodeFactory &factory);
    virtual ~RootNode();

    xmlNodePtr get_at(TNodeIndex idx)
    {
        return treeTable.get(idx);
    }

    virtual bool equals(INode *other);

    virtual xmlNodePtr get_inner();

    virtual INode *get_parent();

    virtual INode *get_left();

    virtual INode *get_right();

    virtual void dump(std::ostream &os, int level) const;
};

}

#endif
