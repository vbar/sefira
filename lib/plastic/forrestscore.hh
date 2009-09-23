#ifndef plastic_forrestscore_hh
#define plastic_forrestscore_hh

#include <map>
#include "graphedge.hh"
#include "relresult.hh"

namespace plastic {

class ForrestScore
{
private:
    typedef std::map<GraphEdge, RelResult> TMap;

    TMap score;

public:
    // asking for edge which hadn't been scored yet is an error
    RelResult get(const GraphEdge &e) const;

    void insert(const GraphEdge &e, const RelResult &r);
};

}

#endif
