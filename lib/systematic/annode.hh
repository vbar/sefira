#ifndef systematic_annode_hh
#define systematic_annode_hh

#include <libxml/tree.h>
#include "inode.hh"
#include "nodeindex.hh"

namespace systematic {

class INodeFactory;

// Metadata ("annotations") common to all binary nodes.
class AnNode : public INode
{
private:
    TNodeIndex size;
    TNodeIndex inorder;

public:
    AnNode();

    virtual bool is_leaf();

    virtual TNodeIndex get_size();

    virtual TNodeIndex get_inorder();

    virtual TNodeIndex fill(TNodeIndex before);

protected:
    INode *make_real_left(INodeFactory &factory, xmlNodePtr inner);
    INode *make_real_right(INodeFactory &factory, xmlNodePtr inner);

    bool is_equal(INode *other);
};

}

#endif
