#ifndef systematic_forrest_hh
#define systematic_forrest_hh

#include <ostream>
#include <utility>
#include "envelope.hh"
#include "forrestletter.hh"
#include "nodeindex.hh"

namespace systematic {

typedef std::pair<TNodeIndex, TNodeIndex> TKernel;

class INode;

class Forrest : private Envelope<ForrestLetter>
{
    friend std::ostream &operator<<(std::ostream &os, const Forrest &f);
public:
    Forrest(INode *node);
    Forrest(INode *left, INode *right);
    Forrest(const Forrest &other);

private:
    Forrest();

public:
    bool operator<(const Forrest &other) const
    {
        return (*get_letter()) < (*(other.get_letter()));
    }

    bool operator==(const Forrest &other) const
    {
        return (*get_letter()) == (*(other.get_letter()));
    }

    bool is_empty() const
    {
        return get_letter()->treeTops.empty();
    }

    bool is_tree() const
    {
        return get_letter()->treeTops.size() == 1;
    }

    // can't be called on empty forrest
    INode *get_front() const;

    // can't be called on empty forrest
    INode *get_back() const;

    // can't be called on empty forrest
    TKernel get_kernel() const;

    // can't be called on empty forrest
    INode *pop_front_root();

    // can't be called on empty forrest
    INode *pop_back_root();

    // can't be called on empty forrest
    INode *pop_front_tree();

    // can't be called on empty forrest
    INode *pop_back_tree();

    static Forrest emptyForrest;

private:
    void do_insert(const Forrest &other);
    void ensure_single();
};

std::ostream &operator<<(std::ostream &os, TKernel k);

std::ostream &operator<<(std::ostream &os, const Forrest &f);

}

#endif
