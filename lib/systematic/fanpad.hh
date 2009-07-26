#ifndef systematic_fanpad_hh
#define systematic_fanpad_hh

#include <map>
#include <ostream>
#include "forrest.hh"
#include "answer.hh"

namespace systematic {

// maps a forrest (one forrest, the other is fixed by the user of this
// class) to LCS.
class FanPad
{
    friend std::ostream &operator<<(std::ostream &os, const FanPad &f);
private:
    typedef std::map<TKernel, Answer> TMap;

    TMap pad;

public:
    // trying to retrieve an answer which hasn't been inserted before
    // returns an empty one
    Answer get(TKernel f) const;

    void set(TKernel f, const Answer &a);

    void clear()
    {
        pad.clear();
    }
};

std::ostream &operator<<(std::ostream &os, const FanPad &f);

}

#endif
