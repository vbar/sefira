#ifndef straight_lcsmemo_hh
#define straight_lcsmemo_hh

#include <map>
#include <utility>
#include "relresult.hh"
#include "forrest.hh"

namespace straight {

class LcsMemo
{
private:
    typedef std::map<std::pair<TKernel, TKernel>, RelResult> TMap;

    TMap memo;

public:
    const RelResult *get(TKernel f, TKernel g) const;

    void insert(TKernel f, TKernel g, RelResult a);
};

}

#endif
