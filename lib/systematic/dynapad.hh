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
    typedef std::pair<Forrest, Forrest> TIndex;
    typedef std::map<TIndex, Answer> TMap;

    TMap map;

public:
    Answer get(const Forrest &f, const Forrest &g) const;

    const Answer *try_get(const Forrest &f, const Forrest &g) const;

    void set(const Forrest &f, const Forrest &g, const Answer &a);
};

}

#endif

