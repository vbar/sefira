#ifndef plastic_matchmap_hh
#define plastic_matchmap_hh

#include <map>
#include <libxml/tree.h>
#include "compare.hh"
#include "nodeindex.hh"

namespace plastic {

class MatchMap
{
private:
    typedef std::map<xmlNodePtr, TNodeIndex> TMatchMap;
    typedef std::map<xmlNodePtr, TNodeIndex, less_xmlNodePtr> TDensityMap;

    TMatchMap matchMap;
    TNodeIndex otherTreeSize;

public:
    // trees must stay valid for the lifetime of the constructed object
    MatchMap(xmlNodePtr tree1, xmlNodePtr tree2);

    TNodeIndex get_count(xmlNodePtr n) const;

    TNodeIndex get_other_tree_size() const
    {
        return otherTreeSize;
    }

private:
    TNodeIndex fill_density(xmlNodePtr t, TDensityMap &d, TNodeIndex sz);

    TNodeIndex compute_match_count(xmlNodePtr n, const TDensityMap &d);

    void insert(xmlNodePtr n, TNodeIndex idx);
};

}

#endif
