#include "dynapad.hh"
#include <sstream>
#include <stdexcept>

#define NOTRACE
#include "trace.hh"

namespace systematic {

RelResult DynaPad::get(TKernel f, TKernel g) const
{
    TMap::const_iterator i = map.find(TIndex(f, g));
    if (i == map.end()) {
        std::stringstream ss;
	ss << "solution to (" << f << ", " << g << ") locally unknown";
	throw std::invalid_argument(ss.str());
    }

    TRACE1("dynaPad(" << f << ", " << g << ") is " << i->second.get_score());
    return i->second;
}

const RelResult *DynaPad::try_get(TKernel f, TKernel g) const
{
    TMap::const_iterator i = map.find(TIndex(f, g));
    if (i == map.end()) {
        TRACE1("answer to (" << f << ", " << g << ") locally unknown");
	return 0;
    } else {
        TRACE1("dynaPad(" << f << ", " << g << ") -> " << i->second.get_score());
	return &(i->second);
    }
}

void DynaPad::set(TKernel f, TKernel g, const RelResult &rr)
{
    TRACE1("dynaPad(" << f << ", " << g << ") := " << rr.get_score());
    map[TIndex(f, g)] = rr;
}

}
