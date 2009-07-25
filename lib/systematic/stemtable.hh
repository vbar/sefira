#ifndef systematic_stemtable_hh
#define systematic_stemtable_hh

#include <map>
#include <ostream>
#include <utility>
#include "answer.hh"

namespace systematic {

class INode;

// maps pairs of (binary) nodes to the LCS of the forrests under them
class StemTable
{
    friend std::ostream &operator<<(std::ostream &os, const StemTable &t);
private:
    typedef std::map<std::pair<INode *, INode *>, Answer> TMap;

    TMap table;

    Answer get(INode *v, INode *w) const;

    void insert(INode *v, INode *w, const Answer &a);

public:

    // trying to retrieve an answer which hasn't been inserted before
    // is an error
    Answer get(INode *v, INode *w, bool swap) const
    {
        return swap ? get(w, v) : get(v, w);
    }

    void insert(INode *v, INode *w, bool swap, const Answer &a)
    {
        if (swap) {
	    insert(w, v, a);
	} else {
	    insert(v, w, a);
	}
    }
};

std::ostream &operator<<(std::ostream &os, const StemTable &t);

}

#endif
