#ifndef systematic_inode_hh
#define systematic_inode_hh

#include <ostream>
#include <libxml/tree.h>
#include "nodeindex.hh"

namespace systematic {

// an interface of the binary node (either backed by a real XML node,
// or an extra one inserted to make the tree binary)
class INode
{
public:
    // deletes the subtree
    virtual ~INode();

    virtual bool is_leaf() = 0;

    virtual TNodeIndex get_size() = 0;

    virtual TNodeIndex get_inorder() = 0;

    // other must not be null
    virtual bool equals(INode *other) = 0;

    virtual xmlNodePtr get_inner() = 0;

    virtual INode *get_parent() = 0;

    virtual INode *get_left() = 0;

    virtual INode *get_right() = 0;

    virtual void dump(std::ostream &os, int level) const = 0;

    virtual TNodeIndex fill(TNodeIndex before) = 0;
};

std::ostream &operator<<(std::ostream &os, const INode &node);

void start_line(std::ostream &os, int level);

void cond_dump(const INode *child, std::ostream &os, int level);

}

#endif
