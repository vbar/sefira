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

std::ostream &operator<<(std::ostream &os, const PathSet &p)
{
    os << "{";
    std::string delim = " ";
    for (PathSet::TPath::const_iterator iter = p.path.begin();
	 iter != p.path.end();
	 ++iter)
    {
        os << delim;
	delim = ", ";
	os << "0x" << std::hex << *iter;
    }

    os << " }";
    return os;
}

}
