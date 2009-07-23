#include "directory.hh"
#include "doubleidx.hh"
#include "graphedge.hh"
#include "xutil.hh"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <assert.h>

namespace optimistic {

struct less_DoubleIdx
{
    bool operator()(const DoubleIdx &double_idx, TNodeIndex tail_idx) const 
    {
        return double_idx.get_in() < tail_idx;
    }

    bool operator()(TNodeIndex tail_idx, const DoubleIdx &double_idx) const 
    {
        return tail_idx < double_idx.get_in();
    }
};

Directory::TAddress Directory::emptyAddress;

Directory::Directory(xmlNodePtr doc_elem):
    docElem(doc_elem)
{
    if (!doc_elem)
    {
	throw std::invalid_argument("Directory: no document element");
    }

    TNodeIndex start_idx = 1;
    TPartialMap part;
    TNodeIndex last = fill(doc_elem, start_idx, part); 
    nodeCount = (last - start_idx) / 2;
    assert(part.empty());
}

TNodeIndex Directory::find_first(xmlNodePtr node, TNodeIndex tail_idx,
				 TNodeIndex head_idx) const
{
    // should work w/o cast - a library bug?
    TFullMap::const_iterator p = const_cast<TFullMap &>(fullMap).find(node);
    if (p == fullMap.end())
    {
        return 0;
    }

    TAddress::const_iterator q = lower_bound(p->second,
					     tail_idx);
    if ((q != p->second.end()) &&  (q->get_out() <= head_idx))
    {
	TNodeIndex ti = q->get_in();
	if (!ti) {
	    throw std::logic_error("zero incoming index");
	}

	return ti;
    }

    return 0;
}

Directory::TAddress::const_iterator Directory::lower_bound(
    const TAddress &address, TNodeIndex tail_idx)
{
    return std::lower_bound(address.begin(),
	address.end(), tail_idx, less_DoubleIdx());
}

Directory::TAddressRange Directory::find(xmlNodePtr node,
    TNodeIndex tail_idx) const
{
    // should work w/o cast - a library bug?
    TFullMap::const_iterator ni = const_cast<TFullMap &>(fullMap).find(node);
    if (ni == fullMap.end())
    {
        return TAddressRange(emptyAddress.begin(), emptyAddress.end());
    }

    return TAddressRange(
        lower_bound(ni->second, tail_idx),
	ni->second.end());
}

TNodeIndex Directory::fill(xmlNodePtr node, TNodeIndex start_idx,
			   TPartialMap &part)
{
    insert_tail(node, start_idx, part);
    TNodeIndex idx = start_idx + 1;
    xmlNodePtr ch = node->children;
    while (ch) {
        idx = fill(ch, idx, part);
	ch = ch->next;
    }

    set_head(node, idx, part);
    return idx + 1;
}

void Directory::insert_tail(xmlNodePtr node, TNodeIndex tail_idx,
    TPartialMap &part)
{
    if (inverseDict.size() <= tail_idx)
    {
	inverseDict.resize(2 * tail_idx + 5);
    }

    inverseDict[tail_idx] = node;

    TFullMap::iterator p = fullMap.find(node);
    if (p == fullMap.end())
    {
        std::pair<TFullMap::iterator, bool> inres = fullMap.insert(
	    TFullMap::value_type(node, TAddress()));
	assert(inres.second);
	p = inres.first;
    }

    assert(p != fullMap.end());

    DoubleIdx didx(tail_idx);
    p->second.push_back(didx);
    part.insert(TPartialMap::value_type(node, didx.get_payload()));
}

void Directory::set_head(xmlNodePtr node, TNodeIndex head_idx,
    TPartialMap &part)
{
    TPartialMap::iterator p = part.find(node);
    assert(p != part.end());
    p->second->second = head_idx;
    part.erase(p);
}

std::ostream &operator<<(std::ostream &os, const Directory &d)
{
    os << "{\n";
    std::string delim = "\t";
    for (Directory::TFullMap::const_iterator i = d.fullMap.begin();
	 i != d.fullMap.end();
	 ++i)
    {
        os << delim;
	delim = ",\n\t";
        os << i->first << ": ";

	os << '[';
	std::string ld = " ";
	for (Directory::TAddress::const_iterator j = i->second.begin();
	     j != i->second.end();
	     ++j)
	{
	    os << ld;
	    ld = ", ";
	    os << *j;
	}

	os << " ]";
    }

    os << "\n}, [";

    std::string ld = " ";
    for (Directory::TInverseDict::const_iterator j = d.inverseDict.begin();
	 j != d.inverseDict.end();
	 ++j)
    {
        os << ld;
	ld = ", ";
	os << *j;
    }

    os << " ]";
    return os;
}

}
