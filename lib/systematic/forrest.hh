#ifndef systematic_forrest_hh
#define systematic_forrest_hh

#include "envelope.hh"
#include "forrestletter.hh"
#include <ostream>

namespace systematic {

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
    INode *get_left() const;

    // can't be called on empty forrest
    INode *get_right() const;

    INode *pop_left_root();

    INode *pop_right_root();

    INode *pop_left_tree();

    INode *pop_right_tree();

    static Forrest emptyForrest;

private:
    void do_insert(const Forrest &other);
    void ensure_single();
};

std::ostream &operator<<(std::ostream &os, const Forrest &f);

}

#endif
