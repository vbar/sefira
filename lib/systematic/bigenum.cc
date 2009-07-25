#include "bigenum.hh"
#include "bigleftenum.hh"
#include "bigrightenum.hh"

namespace systematic {

static BigEnumBase *make_letter(INode *parent, bool twist)
{
    if (twist) {
        return new BigRightEnum(parent);
    } else {
        return new BigLeftEnum(parent);
    }
}

BigEnum::BigEnum(INode *parent, bool twist):
    Envelope<BigEnumBase>(make_letter(parent, twist))
{
}

}
