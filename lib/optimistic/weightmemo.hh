#ifndef optimistic_weightmemo_hh
#define optimistic_weightmemo_hh

#include <map>
#include <utility>
#include <libxml/tree.h>
#include "relresult.hh"
#include "graphedge.hh"

namespace optimistic {

class WeightMemo
{
private:
    typedef std::map<TGraphPoint, RelResult> TMap;

    TMap memo;

public:
    const RelResult *get(const GraphEdge &e) const;

    void insert(const GraphEdge &e, RelResult r);
};

}

#endif
