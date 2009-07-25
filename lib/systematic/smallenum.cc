#include "smallenum.hh"
#include "smallleftenum.hh"
#include "smallrightenum.hh"

namespace systematic {

static SmallEnumBase *make_letter(bool twist)
{
    if (twist) {
        return new SmallLeftEnum();
    } else {
        return new SmallRightEnum();
    }
}

SmallEnum::SmallEnum(INode *root, bool twist):
    Envelope<SmallEnumBase>(make_letter(twist))
{
    get_letter()->init(root);
}

}
