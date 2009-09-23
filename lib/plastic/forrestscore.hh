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
    const RelResult *get(const GraphEdge &e) const;

    void insert(const GraphEdge &e, const RelResult &r);

    void set(const GraphEdge &e, const RelResult &r);
};

}

#endif
