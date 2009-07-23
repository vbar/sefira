#ifndef optimistic_scoreboard_hh
#define optimistic_scoreboard_hh

#include <map>
#include <iostream>
#include "graphedge.hh"
#include "relresult.hh"

namespace optimistic {

class ScoreBoard
{
private:
    typedef std::map<GraphEdge, RelResult> TMap;

    TMap scoreMap;

public:
    friend std::ostream &operator<<(std::ostream &os, const ScoreBoard &b);

    RelResult get(const GraphEdge &e) const;

    void insert(const GraphEdge &e, RelResult r);
};

std::ostream &operator<<(std::ostream &os, const ScoreBoard &b);

}

#endif
