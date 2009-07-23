#include "doubleidx.hh"
#include <stdexcept>

namespace optimistic {

DoubleIdx::DoubleIdx(TNodeIndex in)
{
    if (in < 1)
    {
	throw std::invalid_argument("incoming index must be positive");
    }

    TLetter *payload = get_letter();
    payload->first = in;
    payload->second = 0;
}

DoubleIdx::DoubleIdx(const DoubleIdx &other):
    Envelope<TLetter>(other)
{
}

std::ostream &operator<<(std::ostream &os, const DoubleIdx &d)
{
    os << '(' << d.get_in() << ", " << d.get_out() << ')';
    return os;
}

}
