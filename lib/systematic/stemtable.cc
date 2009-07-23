#include "stemtable.hh"
#include "forrest.hh"
#include "inode.hh"
#include <sstream>
#include <stdexcept>
#include <string>

#define NOTRACE
#include "trace.hh"

namespace systematic {

Answer StemTable::get(INode *v, INode *w) const
{
    TMap::const_iterator i = table.find(TMap::key_type(v, w));
    if (i == table.end()) {
        std::stringstream ss;
	ss << "stemTable has no (" << v << ", " << w << ')';
        throw std::invalid_argument(ss.str());
    }

    TRACE1("stemTable(" << v << ", " << w << ") is " << i->second);
    return i->second;
}

void StemTable::insert(INode *v, INode *w, const Answer &a)
{
    TRACE1("stemTable(" << v << ", " << w << ") := " << a);
    if (v->is_leaf() || w->is_leaf()) {
        if (a.get_score()) {
	    throw std::invalid_argument("non-empty subtree under leaf");
	}
    }

    std::pair<TMap::iterator, bool> inres = table.insert(
	TMap::value_type(TMap::key_type(v, w), a));
    if (!inres.second) {
        throw std::logic_error("repeated insert");
    }
}

std::ostream &operator<<(std::ostream &os, const StemTable &t)
{
    os << "{\n";
    std::string delim = "\t";
    for (StemTable::TMap::const_iterator iter = t.table.begin();
	 iter != t.table.end();
	 ++iter)
    {
	INode *v = iter->first.first;
	INode *w = iter->first.second;
	if (!v->is_leaf() && !w->is_leaf()) {
	    os << delim;
	    delim = ",\n\t";
	    Forrest f(v->get_left(), v->get_right());
	    Forrest g(w->get_left(), w->get_right());
	    os << '[' << f << " x " << g << "]: " << iter->second.get_score();
	}
    }

    os << "\n}";

    return os;
}

}
