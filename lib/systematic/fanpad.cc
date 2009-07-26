#include "fanpad.hh"
#include <iostream>
#include <sstream>
#include <string>

#define NOTRACE
#include "trace.hh"

namespace systematic {

RelResult FanPad::get(TKernel f) const
{
    TMap::const_iterator i = pad.find(f);
    if (i == pad.end()) {
        TRACE1("fanPad(" << f << ") is empty");
        return RelResult();
    } else {
        TRACE1("fanPad(" << f << ") is " << i->second.get_score());
        return i->second;
    }
}

void FanPad::set(TKernel f, const RelResult &rr)
{
    TRACE1("fanPad(" << f << ") := " << rr.get_score());
    pad[f] = rr;
}

std::ostream &operator<<(std::ostream &os, const FanPad &f)
{
    os << "{\n";
    std::string delim = "\t";
    for (FanPad::TMap::const_iterator i = f.pad.begin();
	 i != f.pad.end();
	 ++i)
    {
        os << delim;
	delim = ",\n\t";
        os << i->first << ": " << i->second.get_score();
    }

    os << "\n}";
    return os;
}

}
