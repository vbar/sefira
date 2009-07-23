#include "answer.hh"
#include "xutil.hh"
#include <algorithm>
#include <stdexcept>
#include <string>

TNodeIndex Answer::get_score() const
{
    TNodeIndex s = static_cast<TNodeIndex>(get_letter()->size());
    if (s != get_letter()->size())
    {
	throw std::range_error("node set too big");
    }

    return s;
}

bool Answer::has_node(xmlNodePtr node) const
{
    const TLetter *l = get_letter();
    TLetter::const_iterator iter = l->find(node);
    return iter != l->end();
}

void Answer::insert(xmlNodePtr node)
{
    ensure_single();
    get_letter()->insert(node);
}

void Answer::insert(const Answer &other)
{
    ensure_single();
    do_insert(other);
}

void Answer::do_insert(const Answer &other)
{
    for (TLetter::const_iterator iter = other.get_letter()->begin();
	 iter != other.get_letter()->end();
	 ++iter)
    {
        get_letter()->insert(*iter);
    }
}

void Answer::ensure_single()
{
    if (is_shared())
    {
        Answer spare;
	spare.do_insert(*this);
	swap(spare);
    }
}

std::ostream &operator<<(std::ostream &os, const Answer &a)
{
    if (!a.get_score()) {
        os << '0';
    } else {
	os << "{\n";
	std::string delim = "\t";
	for (Answer::TLetter::const_iterator iter = a.get_letter()->begin();
	     iter != a.get_letter()->end();
	     ++iter)
	{
	    os << delim;
	    delim = ",\n\t";
	    os << xutil::get_node_name(*iter) << " at " << *iter;
	}

	os << "\n}";
    }

    return os;
}
