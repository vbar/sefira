#ifndef systematic_bigleftenum_hh
#define systematic_bigleftenum_hh

#include "bigenum.hh"

namespace systematic {

class INode;

class BigLeftEnum : public BigEnum
{
public:
    BigLeftEnum(INode *parent);
};

}

#endif

