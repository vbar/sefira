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

public:
    // trees must stay valid for the lifetime of the constructed object
    MatchMap(xmlNodePtr tree1, xmlNodePtr tree2);

    TNodeIndex get_count(xmlNodePtr n) const;

private:
    void fill_density(xmlNodePtr t, TDensityMap &d);

    TNodeIndex compute_match_count(xmlNodePtr n, const TDensityMap &d);

    void insert(xmlNodePtr n, TNodeIndex idx);
};

}

#endif
