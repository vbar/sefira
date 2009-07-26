#ifndef systematic_dynapad_hh
#define systematic_dynapad_hh

#include <map>
#include <utility>
#include "answer.hh"
#include "forrest.hh"

namespace systematic {

class DynaPad
{
private:
    typedef std::pair<TKernel, TKernel> TIndex;
    typedef std::map<TIndex, Answer> TMap;

    TMap map;

public:
    Answer get(TKernel f, TKernel g) const;

    const Answer *try_get(TKernel f, TKernel g) const;

    void set(TKernel f, TKernel g, const Answer &a);
};

}

#endif

