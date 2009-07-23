#include "nodefactory.hh"
#include "ghostnode.hh"
#include "realnode.hh"

namespace systematic {

INode *NodeFactory::create_backed(INode &parent, xmlNodePtr inner)
{
    return new RealNode(*this, parent, inner);
}

INode *NodeFactory::create_ghost(INode &parent, xmlNodePtr left)
{
    return new GhostNode(*this, parent, left);
}

}
