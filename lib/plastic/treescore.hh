#ifndef plastic_treescore_hh
#define plastic_treescore_hh

#include <map>
#include <utility>
#include <libxml/tree.h>
#include "answer.hh"

namespace plastic {

class TreeScore
{
private:
    typedef std::map<std::pair<xmlNodePtr, xmlNodePtr>, Answer> TMap;

    TMap score;

public:
    const Answer *get(xmlNodePtr f, xmlNodePtr g) const;

    void insert(xmlNodePtr f, xmlNodePtr g, const Answer &a);
};

}

#endif
