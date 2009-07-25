#include "fanpad.hh"
#include <iostream>
#include <sstream>
#include <string>

#define NOTRACE
#include "trace.hh"

namespace systematic {

Answer FanPad::get(const Forrest &f) const
{
    TMap::const_iterator i = pad.find(f);
    if (i == pad.end()) {
        TRACE1("fanPad(" << f << ") is empty");
        return Answer();
    } else {
        TRACE1("fanPad(" << f << ") is " << i->second.get_score());
        return i->second;
    }
}

void FanPad::set(const Forrest &f, const Answer &a)
{
    TRACE1("fanPad(" << f << ") := " << a.get_score());
    pad[f] = a;
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