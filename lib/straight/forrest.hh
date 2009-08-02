#ifndef straight_forrest_hh
#define straight_forrest_hh

#include <iostream>
#include <vector>
#include <libxml/tree.h>
#include "envelope.hh"
#include "nodeindex.hh"

namespace straight {

typedef std::pair<TNodeIndex, TNodeIndex> TKernel;

class Forrest : private Envelope<std::vector<xmlNodePtr> >
{
public:
    bool operator<(const Forrest &other) const;

    bool operator==(const Forrest &other) const;

    bool is_empty() const
    {
        return get_letter()->empty();
    }

    TLetter::size_type size() const { return get_letter()->size(); }
  
    bool is_tree() const
    {
        return get_letter()->size() == 1;
    }

    // can't be called on empty forrest
    xmlNodePtr get_front() const
    {
	return get_letter()->front();
    }

    // can't be called on empty forrest
    xmlNodePtr get_back() const
    {
	return get_letter()->back();
    }

    void insert(xmlNodePtr node);

    // can't be called on empty forrest
    xmlNodePtr pop_front_root();

    // can't be called on empty forrest
    xmlNodePtr pop_front_tree();

    // can't be called on empty forrest
    xmlNodePtr pop_back_root();

    // can't be called on empty forrest
    xmlNodePtr pop_back_tree();

private:
    void do_insert(const Forrest &other);
    void ensure_single();
};

std::ostream &operator<<(std::ostream &os, const Forrest &f);

}

#endif
