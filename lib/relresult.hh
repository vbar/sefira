#ifndef relresult_hh
#define relresult_hh

#include <iostream>
#include <vector>
#include "envelope.hh"
#include "nodeindex.hh"

class RelResult : private Envelope<std::vector<TNodeIndex> >
{
    friend std::ostream &operator<<(std::ostream &os, const RelResult &a);
public:
    typedef std::vector<TNodeIndex> TSet;

    TNodeIndex get_score() const;

    RelResult crop(TNodeIndex tail_idx) const;

    void insert(TNodeIndex tail_idx);
    void insert(const RelResult &other);

    TSet::const_iterator begin() const
    {
        return get_letter()->begin();
    }

    TSet::const_iterator end() const
    {
        return get_letter()->end();
    }

private:
    void do_insert(const RelResult &other);
    void ensure_single();
};

std::ostream &operator<<(std::ostream &os, const RelResult &rr);

#endif
