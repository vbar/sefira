#include "smallenum.hh"
#include <sstream>
#include <stdexcept>
#include <string>

namespace systematic {

SmallEnum::SmallEnum():
    leafCount(0),
    maxSize(0)
{
}

SmallEnum::~SmallEnum()
{
    delete [] maxSize;
}

TNodeIndex SmallEnum::get_max_size(TNodeIndex n) const
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

SmallEnum::TRange SmallEnum::get_range(TNodeIndex n, TNodeIndex sz) const
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

void SmallEnum::cond_add_index(const Forrest &forrest, INode *leaf,
        TNodeIndex sz, const TLeafMap &map, TKnownSet &set)
{
    if (set.find(forrest) != set.end()) {
        return;
    }

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

void SmallEnum::add_leaf(INode *node, TLeafMap &map)
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

std::ostream &operator<<(std::ostream &os, const SmallEnum &se)
{
    os << "{\n";
    std::string delim = "\t";
    for (SmallEnum::TMatrix::const_iterator iter = se.matrix.begin();
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
