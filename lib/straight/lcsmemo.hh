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
    typedef std::map<std::pair<Forrest, Forrest>, Answer> TMap;

    TMap memo;


public:
    const Answer *get(const Forrest &f, const Forrest &g) const;

    void insert(const Forrest &f, const Forrest &g, Answer a);
};

}

#endif
