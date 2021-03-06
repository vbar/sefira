#include "matchmap.hh"
#include <stdexcept>
#include <utility>

#define NOTRACE
#include "trace.hh"

namespace plastic {

MatchMap::MatchMap(xmlNodePtr tree1, TNodeIndex sz1, xmlNodePtr tree2)
{
    TDensityMap density;
    otherTreeSize = fill_density(tree2, density, 0);
    sizeOk = sz1 >= otherTreeSize;
    if (sizeOk) {
        compute_match_count(tree1, density);
    }
}

TNodeIndex MatchMap::fill_density(xmlNodePtr t, TDensityMap &d, TNodeIndex sz)
{
    TRACE1("enter fill_density(" << t << ", ...)");
    std::pair<TDensityMap::iterator, bool> inres = d.insert(
        TDensityMap::value_type(t, 1));
    if (!inres.second) {
	++(inres.first->second);
    }

    ++sz;

    xmlNodePtr ch = t->children;
    while (ch) {
        sz = fill_density(ch, d, sz);
	ch = ch->next;
    }

    return sz;
}

TNodeIndex MatchMap::get_count(xmlNodePtr n) const
{
    TMatchMap::const_iterator i = matchMap.find(n);
    if (i == matchMap.end()) {
        throw std::invalid_argument("unknown node");        
    }

    return i->second;
}

TNodeIndex MatchMap::compute_match_count(xmlNodePtr n, const TDensityMap &d)
{
    TRACE1("enter compute_match_count(" << n << ", ...)");
    TMatchMap::const_iterator i = matchMap.find(n);
    if (i != matchMap.end()) {
        return i->second;
    }

    // should work w/o cast - a library bug?
    TDensityMap::const_iterator j = const_cast<TDensityMap &>(d).find(n);
    TNodeIndex count = (j == d.end()) ? 0 : j->second;

    xmlNodePtr ch = n->children;
    while (ch) {
        count += compute_match_count(ch, d);
	ch = ch->next;
    }
    
    insert(n, count);
    return count;
}

void MatchMap::insert(xmlNodePtr n, TNodeIndex idx)
{
    std::pair<TMatchMap::iterator, bool> inres = matchMap.insert(
        TMatchMap::value_type(n, idx));
    if (!inres.second) {
        throw std::logic_error("MatchMap: repeated insert");
    }
}

}
