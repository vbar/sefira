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
    bool sizeOk;

public:
    // trees must stay valid for the lifetime of the constructed
    // object
    // sz1 is the size of tree1; when it's smaller than the size of
    // tree2, the MatchMap instance is constructed invalid and the
    // only method which can be called on it is is_size_ok (which
    // returns false). when is_size_ok returns true, it means size of
    // tree1 is at least size of tree2 (which is available as the
    // return value of get_other_tree_size) and the new instance can
    // be used.
    MatchMap(xmlNodePtr tree1, TNodeIndex sz1, xmlNodePtr tree2);

    bool is_size_ok() const
    {
        return sizeOk;
    }

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
