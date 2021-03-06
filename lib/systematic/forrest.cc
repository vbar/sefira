#include "forrest.hh"
#include "inode.hh"
#include <algorithm>
#include <sstream>
#include <stdexcept>

#define NOTRACE
#include "trace.hh"

namespace systematic {

Forrest Forrest::emptyForrest;

Forrest::Forrest(INode *node):
    Envelope<ForrestLetter>(new ForrestLetter(node, 0))
{
    if (!node) {
        throw std::invalid_argument("can't construct forrest from NULL");
    }
}

Forrest::Forrest(INode *left, INode *right):
    Envelope<ForrestLetter>(new ForrestLetter(left, right))
{
    if (!left && !right) {
        throw std::invalid_argument("use emptyForrest for an empty forrest");
    }
}

Forrest::Forrest(const Forrest &other):
    Envelope<ForrestLetter>(other)
{
}

Forrest::Forrest():
    Envelope<ForrestLetter>(new ForrestLetter())
{
}

INode *Forrest::get_front() const
{
    const ForrestLetter *letter = get_letter();
    if (letter->treeTops.empty()) {
        throw std::logic_error("empty forrest doesn't have leftmost tree");
    }

    return letter->treeTops[0];
}

INode *Forrest::get_back() const
{
    const ForrestLetter *letter = get_letter();
    if (letter->treeTops.empty()) {
        throw std::logic_error("empty forrest doesn't have rightmost tree");
    }

    return letter->treeTops.back();
}

TKernel Forrest::get_kernel() const
{
    const ForrestLetter *letter = get_letter();
    if (letter->treeTops.empty()) {
        throw std::logic_error("empty forrest doesn't need a kernel");
    }

    return TKernel(letter->treeTops[0]->get_inorder(),
		   letter->treeTops.back()->get_inorder());
}

INode *Forrest::pop_front_root()
{
    typedef ForrestLetter::TTreeTops TTreeTops;

    TRACE1("enter pop_front_root of " << *this);

    ForrestLetter *old_letter = get_letter();
    TTreeTops::const_iterator iter = old_letter->treeTops.begin();
    if (iter == old_letter->treeTops.end()) {
        throw std::logic_error("can't pop leftmost root from empty forrest");
    }

    INode *node = *iter;
    ++iter;

    Forrest spare;
    ForrestLetter *new_letter = spare.get_letter();

    new_letter->append_children(node);

    while (iter != old_letter->treeTops.end()) {
        new_letter->treeTops.push_back(*iter);
	++iter;
    }
  
    swap(spare);

    TRACE1("exit pop_front_root: " << *this << " returns " << node);
    return node;
}

INode *Forrest::pop_back_root()
{
    ensure_single();
    return get_letter()->pop_back_root();
}

INode *Forrest::pop_front_tree()
{
    typedef ForrestLetter::TTreeTops TTreeTops;

    ForrestLetter *old_letter = get_letter();
    TTreeTops::const_iterator iter = old_letter->treeTops.begin();
    if (iter == old_letter->treeTops.end()) {
        throw std::logic_error("can't pop leftmost tree from empty forrest");
    }

    INode *node = *iter;
    ++iter;

    Forrest spare;
    ForrestLetter *new_letter = spare.get_letter();

    while (iter != old_letter->treeTops.end()) {
        new_letter->treeTops.push_back(*iter);
	++iter;
    }
  
    swap(spare);

    return node;
}

INode *Forrest::pop_back_tree()
{
    ensure_single();
    return get_letter()->pop_back_tree();
}

void Forrest::do_insert(const Forrest &other)
{
    typedef ForrestLetter::TTreeTops TTreeTops;

    ForrestLetter *nl = get_letter();
    const ForrestLetter *ol = other.get_letter();
    for (TTreeTops::const_iterator iter = ol->treeTops.begin();
	 iter != ol->treeTops.end();
	 ++iter)
    {
        nl->treeTops.push_back(*iter);
    }
}

void Forrest::ensure_single()
{
    if (is_shared())
    {
        Forrest spare;
	spare.do_insert(*this);
	swap(spare);
    }
}

std::ostream &operator<<(std::ostream &os, TKernel k)
{
    os << '(' << k.first << ", " << k.second << ')';
    return os;
}

std::ostream &operator<<(std::ostream &os, const Forrest &f)
{
    f.get_letter()->dump(os);
    return os;
}

}
