#ifndef systematic_dynapad_hh
#define systematic_dynapad_hh

#include <map>
#include <utility>
#include "relresult.hh"
#include "forrest.hh"

namespace systematic {

class DynaPad
{
private:
    typedef std::pair<TKernel, TKernel> TIndex;
    typedef std::map<TIndex, RelResult> TMap;

    TMap map;

public:
    RelResult get(TKernel f, TKernel g) const;

    const RelResult *try_get(TKernel f, TKernel g) const;

    void set(TKernel f, TKernel g, const RelResult &rr);
};

}

#endif

