#include "bigenum.hh"
#include <stdexcept>
#include <string>

namespace systematic {

BigEnum::BigEnum():
    emptyEdge(false)
{
}

BigEnum::~BigEnum()
{
}

TNodeIndex BigEnum::get_size() const
{
    TNodeIndex d = emptyEdge ? 1 : 0;
    TNodeIndex sz = static_cast<TNodeIndex>(seq.size()) + d;
    if (sz - d != seq.size()) {
        throw std::range_error("too many intermediate forrests");
    }

    return sz;
}

Forrest BigEnum::get(TNodeIndex idx)
{
    TEnum::size_type r;
    if (emptyEdge) {
        if (!idx) {
	    return Forrest::emptyForrest;
	}

	r = seq.size() - idx;
    } else {
	r = seq.size() - idx - 1;
    }

    return seq.at(r);
}

std::ostream &operator<<(std::ostream &os, const BigEnum &be)
{
    os << "[\n";
    std::string delim = "\t";
    if (be.emptyEdge) {
        os << delim << "[,]";
	delim = ",\n\t";
    }
    
    for (BigEnum::TEnum::const_reverse_iterator iter = be.seq.rbegin();
	 iter != be.seq.rend();
	 ++iter)
    {
        os << delim << *iter;
	delim = ",\n\t";
    }

    os << "\n]";

    return os;
}

}
