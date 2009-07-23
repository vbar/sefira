#ifndef optimistic_directory_hh
#define optimistic_directory_hh

#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <libxml/tree.h>
#include "compare.hh"
#include "doubleidx.hh"
#include "nodeindex.hh"

namespace optimistic {

class Directory
{
public:
    typedef std::vector<DoubleIdx> TAddress;
    typedef std::pair<TAddress::const_iterator,
		      TAddress::const_iterator> TAddressRange;
    typedef std::map<xmlNodePtr, TAddress, less_xmlNodePtr> TFullMap;

private:
    typedef std::vector<xmlNodePtr> TInverseDict;
    typedef std::map<xmlNodePtr, DoubleIdx::TPayload *> TPartialMap;

    static TAddress emptyAddress;

    xmlNodePtr docElem;

    TInverseDict inverseDict;

    TFullMap fullMap;

    TNodeIndex nodeCount;

public:
    friend std::ostream &operator<<(std::ostream &os, const Directory &d);

    Directory(xmlNodePtr doc_elem);

    xmlNodePtr get_tree() const { return docElem; }

    TNodeIndex get_node_count() const { return nodeCount; }

    TNodeIndex find_first(xmlNodePtr node, TNodeIndex tail_idx,
			  TNodeIndex head_idx) const;

    TFullMap::const_iterator begin() const
    {
        return fullMap.begin();
    }

    TFullMap::const_iterator end() const
    {
        return fullMap.end();
    }

    TAddressRange find(xmlNodePtr node, TNodeIndex tail_idx) const;

    xmlNodePtr recall(TNodeIndex tail_idx)
    {
        return inverseDict.at(tail_idx);
    }

    static TAddress::const_iterator lower_bound(const TAddress &address,
	TNodeIndex tail_idx);

private:
    TNodeIndex fill(xmlNodePtr node, TNodeIndex start_idx, TPartialMap &part);

    void insert_tail(xmlNodePtr node, TNodeIndex tail_idx, TPartialMap &part);

    void set_head(xmlNodePtr node, TNodeIndex head_idx, TPartialMap &part);
};

std::ostream &operator<<(std::ostream &os, const Directory &d);

}

#endif
