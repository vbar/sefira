#include "relresult.hh"
#include <stdexcept>
#include <string>

TNodeIndex RelResult::get_score() const
{
    TNodeIndex s = static_cast<TNodeIndex>(get_letter()->size());
    if (s != get_letter()->size())
    {
	throw std::range_error("subresult too big");
    }

    return s;
}

RelResult RelResult::crop(TNodeIndex tail_idx) const
{
    RelResult cropped;
    for (TSet::const_iterator i = get_letter()->begin();
	 i != get_letter()->end();
	 ++i)
    {
        if (*i >= tail_idx)
	{
	    cropped.get_letter()->push_back(*i);
	}
    }

    return cropped;
}

void RelResult::insert(TNodeIndex tail_idx)
{
    ensure_single();
    get_letter()->push_back(tail_idx);
}

void RelResult::insert(const RelResult &other)
{
    ensure_single();
    do_insert(other);
}

void RelResult::do_insert(const RelResult &other)
{
    for (TSet::const_iterator iter = other.get_letter()->begin();
	 iter != other.get_letter()->end();
	 ++iter)
    {
        get_letter()->push_back(*iter);
    }
}

void RelResult::ensure_single()
{
    if (is_shared())
    {
        RelResult spare;
	spare.do_insert(*this);
	swap(spare);
    }
}

std::ostream &operator<<(std::ostream &os, const RelResult &rr)
{
    os << "[";
    std::string delim = " ";
    for (RelResult::TSet::const_iterator iter = rr.get_letter()->begin();
	 iter != rr.get_letter()->end();
	 ++iter)
    {
        os << delim;
	delim = ", ";
        os << *iter;
    }

    os << " ]";

    return os;
}
