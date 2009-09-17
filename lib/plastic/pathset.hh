#ifndef plastic_pathset_hh
#define plastic_pathset_hh

#include <libxml/tree.h>
#include <iostream>
#include <set>

namespace plastic {

class PathSet
{
private:
    typedef std::set<xmlNodePtr> TPath;
    TPath path;

public:
    friend std::ostream &operator<<(std::ostream &os, const PathSet &p);

    PathSet(xmlNodePtr root, xmlNodePtr leaf);

    bool is_on_path(xmlNodePtr node) const;
};

std::ostream &operator<<(std::ostream &os, const PathSet &p);

}

#endif

