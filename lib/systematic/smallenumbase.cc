#include "smallenumbase.hh"
#include "inode.hh"
#include <sstream>
#include <stdexcept>
#include <string>
#include "string.h"

#define NOTRACE
#include "trace.hh"

namespace systematic {

SmallEnumBase::SmallEnumBase():
    leafCount(0),
    maxSize(0)
{
}

SmallEnumBase::~SmallEnumBase()
{
    delete [] maxSize;
}

void SmallEnumBase::init(INode *root)
{
    TRACE1("SmallEnumBase::init(" << root << ')');
    TLeafMap map;
    fill_leaf_map(root, map);
    leafCount = static_cast<TNodeIndex>(map.size());
    if (leafCount != map.size()) {
        throw std::range_error("SmallEnumBase: too many leaves");
    }

    TKnownSet set;

    maxSize = new TNodeIndex[leafCount];
    memset(maxSize, 0, sizeof(TNodeIndex) * leafCount);

    // inefficient - tries to add some subforrests twice
    Forrest f(root);
    TNodeIndex sz = root->get_size();
    while (sz) {
        add_left_subforrests(f, sz, map, set);
	f.pop_back_root();
	--sz;
    }

    TRACE1("exit SmallEnumBase::init");
}

void SmallEnumBase::add_left_subforrests(const Forrest &forrest,
        TNodeIndex sz,
        const TLeafMap &map,
        TKnownSet &set)
{
    Forrest f(forrest);
    while (sz) {
        add_one(f, sz, map, set);
	f.pop_front_root();
	--sz;
    }
}

TNodeIndex SmallEnumBase::get_max_size(TNodeIndex n) const
{
    if (!n) {
        throw std::invalid_argument("leaf number can't be zero");
    }

    TNodeIndex major = n - 1;
    if (major >= leafCount) {
        throw std::invalid_argument("leaf number out of range");
    }

    return maxSize[major];
}

SmallEnumBase::TRange SmallEnumBase::get_range(TNodeIndex n, TNodeIndex sz) const
{
    if (!n) {
        throw std::invalid_argument("leaf number cannot be zero");
    }

    TNodeIndex major = n - 1;
    TRange rng = matrix.equal_range(TIndex(major, sz));
    if (rng.first == rng.second) {
        std::stringstream ss;
	ss << "small index (" << n << ", " << sz << ") not enumerated";
	throw std::invalid_argument(ss.str());
    }

    return rng;
}

void SmallEnumBase::add_unknown_index(const Forrest &forrest, INode *leaf,
        TNodeIndex sz, const TLeafMap &map, TKnownSet &set)
{
    TLeafMap::const_iterator i = map.find(leaf);
    if (i == map.end()) {
        std::stringstream ss;
        ss << "unknown leaf " << leaf;
	throw std::invalid_argument(ss.str());
    }

    TNodeIndex major = i->second;
    if (sz > maxSize[major]) {
        maxSize[major] = sz;
    }

    set.insert(forrest);

    TIndex idx(major, sz);
    matrix.insert(TMatrix::value_type(idx, forrest));
}

void SmallEnumBase::add_leaf(INode *node, TLeafMap &map)
{
    if (!node) {
        throw std::invalid_argument("leaf can't be null");
    }

    TLeafMap::size_type n = map.size();
    TNodeIndex sn = static_cast<TNodeIndex>(n);
    if (n != sn) {
	throw std::range_error("add_leaf: too many leaves");
    }

    std::pair<TLeafMap::iterator, bool> inres = map.insert(
	TLeafMap::value_type(node, sn));
    if (!inres.second) {
	throw std::logic_error("found repeated node in tree");
    }
}

std::ostream &operator<<(std::ostream &os, const SmallEnumBase &se)
{
    os << "{\n";
    std::string delim = "\t";
    for (SmallEnumBase::TMatrix::const_iterator iter = se.matrix.begin();
	 iter != se.matrix.end();
	 ++iter)
    {
        os << delim;
	delim = ",\n\t";
	TNodeIndex n = iter->first.first + 1;
        os << '(' << n << ", " << iter->first.second;
	os << "): " << iter->second;
    }

    os << "\n}";

    return os;
}

}
