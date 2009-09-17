#ifndef plastic_forrestscore_hh
#define plastic_forrestscore_hh

#include <map>
#include "graphedge.hh"
#include "answer.hh"

namespace plastic {

class ForrestScore
{
private:
    typedef std::map<GraphEdge, Answer> TMap;

    TMap score;

public:
    const Answer *get(const GraphEdge &e) const;

    void insert(const GraphEdge &e, const Answer &a);
};

}

#endif
