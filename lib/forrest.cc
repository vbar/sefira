#include "forrest.hh"
#include <algorithm>
#include <stdexcept>
#include <string>

bool Forrest::operator<(const Forrest &other) const
{
    const TLetter *a = get_letter();
    const TLetter *b = other.get_letter();

    TLetter::size_type ml = std::min(a->size(), b->size());
    for (TLetter::size_type i = 0; i < ml; ++i) {
        if ((*a)[i] != (*b)[i]) {
	    return (*a)[i] < (*b)[i];
	}
    }

    return a->size() < b->size();
}

bool Forrest::operator==(const Forrest &other) const
{
    const TLetter *a = get_letter();
    const TLetter *b = other.get_letter();

    if (a->size() != b->size()) {
        return false;
    }

    for (TLetter::size_type i = 0; i < a->size(); ++i) {
        if ((*a)[i] != (*b)[i]) {
	    return false;
	}
    }

    return true;
}

void Forrest::insert(xmlNodePtr node)
{
    ensure_single();
    get_letter()->push_back(node);
}

xmlNodePtr Forrest::pop_front_root()
{
    TLetter *old_letter = get_letter();
    if (old_letter->empty()) {
        throw std::logic_error("can't pop front from empty forrest");
    }

    Forrest spare;
    TLetter *new_letter = spare.get_letter();

    xmlNodePtr node = (*old_letter)[0];
    xmlNodePtr ch = node->children;
    while (ch) {
        new_letter->push_back(ch);
	ch = ch->next;
    }

    for (TLetter::const_iterator iter = old_letter->begin() + 1;
	 iter != old_letter->end();
	 ++iter) {
        new_letter->push_back(*iter);
    }
  
    swap(spare);
    return node;
}

xmlNodePtr Forrest::pop_front_tree()
{
    TLetter *old_letter = get_letter();
    if (old_letter->empty()) {
        throw std::logic_error("cannot pop front from empty forrest");
    }

    Forrest spare;
    TLetter *new_letter = spare.get_letter();

    for (TLetter::const_iterator iter = old_letter->begin() + 1;
	 iter != old_letter->end();
	 ++iter) {
        new_letter->push_back(*iter);
    }
  
    swap(spare);
    return (*old_letter)[0];
}

xmlNodePtr Forrest::pop_back_root()
{
    ensure_single();
    TLetter *letter = get_letter();
    if (letter->empty()) {
        throw std::logic_error("can't pop back from empty forrest");
    }

    xmlNodePtr node = letter->back();
    letter->pop_back();
    xmlNodePtr ch = node->children;
    while (ch) {
        letter->push_back(ch);
	ch = ch->next;
    }

    return node;
}

xmlNodePtr Forrest::pop_back_tree()
{
    ensure_single();
    TLetter *letter = get_letter();
    if (letter->empty()) {
        throw std::logic_error("cannot pop back from empty forrest");
    }

    xmlNodePtr node = letter->back();
    letter->pop_back();
    return node;
}

void Forrest::do_insert(const Forrest &other)
{
    for (TLetter::const_iterator iter = other.get_letter()->begin();
	 iter != other.get_letter()->end();
	 ++iter)
    {
        get_letter()->push_back(*iter);
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

std::ostream &operator<<(std::ostream &os, const Forrest &f)
{
    os << "[ " << f.size() << "] ";
    return os;
}

