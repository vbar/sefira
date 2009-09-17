#include "pathset.hh"
#include <stdexcept>

namespace plastic {

PathSet::PathSet(xmlNodePtr root, xmlNodePtr leaf)
{
    if (!root)
    {
	throw std::invalid_argument("path root can't be null");
    }

    if (!leaf)
    {
	throw std::invalid_argument("path leaf can't be null");
    }

    while (leaf != root)
    {
        path.insert(leaf);
	leaf = leaf->parent;
    }

    path.insert(root);
}

bool PathSet::is_on_path(xmlNodePtr node) const
{
    TPath::const_iterator i = path.find(node);
    return i != path.end();
}

}
