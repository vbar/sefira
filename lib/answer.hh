#ifndef answer_hh
#define answer_hh

#include <ostream>
#include <set>
#include <libxml/tree.h>
#include "envelope.hh"
#include "nodeindex.hh"

class Answer : private Envelope<std::set<xmlNodePtr> >
{
    friend std::ostream &operator<<(std::ostream &os, const Answer &a);
public:
    TNodeIndex get_score() const;
    bool has_node(xmlNodePtr node) const;

    void insert(xmlNodePtr node);
    void insert(const Answer &other);

private:
    void do_insert(const Answer &other);
    void ensure_single();
};

std::ostream &operator<<(std::ostream &os, const Answer &a);

#endif
