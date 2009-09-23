#ifndef plastic_treescore_hh
#define plastic_treescore_hh

#include <map>
#include <utility>
#include <libxml/tree.h>
#include "relresult.hh"

namespace plastic {

class TreeScore
{
private:
    typedef std::map<std::pair<xmlNodePtr, xmlNodePtr>, RelResult> TMap;

    TMap score;

public:
    const RelResult *get(xmlNodePtr f, xmlNodePtr g) const;

    void update(xmlNodePtr f, xmlNodePtr g, const RelResult &r);
};

}

#endif
