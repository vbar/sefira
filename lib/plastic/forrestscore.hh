#ifndef plastic_forrestscore_hh
#define plastic_forrestscore_hh

#include <map>
#include <ostream>
#include "graphedge.hh"
#include "relresult.hh"

namespace plastic {

// same semantics as optimistic::ScoreBoard, but can't be reused
// (short of templatization) because GraphEdge is different in the two
// algorithms
class ForrestScore
{
private:
    typedef std::map<GraphEdge, RelResult> TMap;

    TMap scoreMap;

public:
    friend std::ostream &operator<<(std::ostream &os, const ForrestScore &fs);

    // asking for edge which hadn't been scored yet is an error
    RelResult get(const GraphEdge &e) const;

    // inserting an already scored edge is an error
    void insert(const GraphEdge &e, const RelResult &r);
};

std::ostream &operator<<(std::ostream &os, const ForrestScore &fs);

}

#endif
