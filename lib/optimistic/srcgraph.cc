#include "srcgraph.hh"
#include <assert.h>

namespace optimistic {

SrcGraph::TGroup SrcGraph::emptyGroup;

const SrcGraph::TGroup &SrcGraph::get_group(const TMap &m,
					    TNodeIndex first)
{
    TMap::const_iterator iter = m.find(first);
    return iter == m.end() ? emptyGroup : iter->second;
}

void SrcGraph::insert_group(TMap &m, TNodeIndex key, const GraphEdge &e)
{
    TMap::iterator iter = m.find(key);
    if (iter == m.end())
    {
        m.insert(TMap::value_type(key, TGroup()));
	iter = m.find(key);
    }

    assert(iter != m.end());
    iter->second.push_back(e);
}

void SrcGraph::dump(std::ostream &os, const TMap &m) const
{
    os << "{\n";
    std::string delim = "\t";
    for (TMap::const_iterator i = m.begin();
	 i != m.end();
	 ++i)
    {
        os << delim;
	delim = ",\n\t";
        os << i->first << ": ";
	os << '[';
	std::string ld = " ";
	for (TGroup::const_iterator j = i->second.begin();
	     j != i->second.end();
	     ++j)
	{
	    os << ld;
	    ld = ", ";
	    os << *j;
	}

	os << " ]";
    }

    os << "\n}";
}

std::ostream &operator<<(std::ostream &os, const SrcGraph &sg)
{
    sg.dump_head(os);
    return os;
}

}
