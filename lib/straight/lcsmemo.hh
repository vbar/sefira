#ifndef straight_lcsmemo_hh
#define straight_lcsmemo_hh

#include <map>
#include <utility>
#include "answer.hh"
#include "forrest.hh"

namespace straight {

class LcsMemo
{
private:
    typedef std::map<std::pair<TKernel, TKernel>, Answer> TMap;

    TMap memo;

public:
    const Answer *get(TKernel f, TKernel g) const;

    void insert(TKernel f, TKernel g, Answer a);
};

}

#endif
