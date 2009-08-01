#ifndef straight_itreeanno_hh
#define straight_itreeanno_hh

#include <libxml/tree.h>

namespace straight {

class ITreeAnno
{
public:
    virtual ~ITreeAnno();

    virtual bool is_light(xmlNodePtr node) const = 0;
};

}

#endif
