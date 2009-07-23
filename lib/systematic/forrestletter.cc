#include "forrestletter.hh"
#include "inode.hh"
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>

#define NOTRACE
#include "trace.hh"

namespace systematic {

ForrestLetter::ForrestLetter(INode *lm, INode *rm)
{
    if ((lm || rm) && (lm == rm)) {
        throw std::invalid_argument("left root must be different from right");
    }

    if (lm) {
        treeTops.push_back(lm);
    }

    if (rm) {
        treeTops.push_back(rm);
    }
}

bool ForrestLetter::operator<(const ForrestLetter &other) const
{
    TTreeTops::size_type ml = std::min(treeTops.size(),
				       other.treeTops.size());
    for (TTreeTops::size_type i = 0; i < ml; ++i) {
        if (treeTops[i] != other.treeTops[i]) {
	    return treeTops[i] < other.treeTops[i];
	}
    }

    return treeTops.size() < other.treeTops.size();
}

bool ForrestLetter::operator==(const ForrestLetter &other) const
{
    if (treeTops.size() != other.treeTops.size()) {
        return false;
    }

    for (TTreeTops::size_type i = 0; i < treeTops.size(); ++i) {
        if (treeTops[i] != other.treeTops[i]) {
	    return false;
	}
    }

    return true;
}

INode *ForrestLetter::pop_right_root()
{
    TRACE1("enter pop_right_root of " << *this);

    if (treeTops.empty()) {
        throw std::invalid_argument("can't pop rightmost root from empty forrest");
    }

    INode *node = treeTops.back();
    treeTops.pop_back();
    append_children(node);

    TRACE1("exit pop_right_root: " << *this << " returns " << node);
    return node;
}

INode *ForrestLetter::pop_right_tree()
{
    if (treeTops.empty()) {
        throw std::invalid_argument("can't pop rightmost tree from empty forrest");
    }

    INode *node = treeTops.back();
    treeTops.pop_back();

    return node;
}

void ForrestLetter::append_children(INode *node)
{
    if (!node) {
        throw std::invalid_argument("can't append NULL");
    }

    INode *ch = node->get_left();
    if (ch) {
        treeTops.push_back(ch);
    }

    ch = node->get_right();
    if (ch) {
        treeTops.push_back(ch);
    }
}

void ForrestLetter::dump(std::ostream &os) const
{
    os << "[ ";
    std::string delim = "";
    for (TTreeTops::const_iterator iter = treeTops.begin();
	 iter != treeTops.end();
	 ++iter)
    {
	os << delim;
	delim = ", ";
	os << *iter;
    }

    os << " ]";
}

std::ostream &operator<<(std::ostream &os, const ForrestLetter &f)
{
    f.dump(os);
    return os;
}

}
