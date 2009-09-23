#ifndef optimistic_scoreboard_hh
#define optimistic_scoreboard_hh

#include <map>
#include <iostream>
#include "graphedge.hh"
#include "relresult.hh"

namespace optimistic {

// same semantics as plastic::ForrestScore, but can't be reused
// (short of templatization) because GraphEdge is different in the two
// algorithms
class ScoreBoard
{
private:
    typedef std::map<GraphEdge, RelResult> TMap;

    TMap scoreMap;

public:
    friend std::ostream &operator<<(std::ostream &os, const ScoreBoard &b);

    // asking for edge which hadn't been scored yet is an error
    RelResult get(const GraphEdge &e) const;

    // inserting an already scored edge is an error
    void insert(const GraphEdge &e, const RelResult &r);
};

std::ostream &operator<<(std::ostream &os, const ScoreBoard &b);

}

#endif
