#ifndef systematic_stemtable_hh
#define systematic_stemtable_hh

#include <map>
#include <ostream>
#include <utility>
#include "relresult.hh"

namespace systematic {

typedef std::pair<TNodeIndex, TNodeIndex> TSubProblem;

class INode;

// maps pairs of nodes (identified by their indices) to the LCS of the
// forrests under them
class StemTable
{
private:
    typedef std::map<TSubProblem, RelResult> TMap;

    TMap table;

public:    
    // trying to retrieve an answer which hasn't been inserted before
    // is an error
    RelResult get(TSubProblem sp) const;

    void insert(TSubProblem sp, const RelResult &rr);
};

}

#endif
