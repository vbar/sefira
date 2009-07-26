#include "stemtable.hh"
#include "forrest.hh"
#include "inode.hh"
#include <sstream>
#include <stdexcept>
#include <string>

#define NOTRACE
#include "trace.hh"

namespace systematic {

RelResult StemTable::get(TSubProblem sp) const
{
    TMap::const_iterator i = table.find(sp);
    if (i == table.end()) {
        std::stringstream ss;
	ss << "stemTable has no " << sp;
        throw std::invalid_argument(ss.str());
    }

    TRACE1("stemTable" << sp << " is " << i->second);
    return i->second;
}

void StemTable::insert(TSubProblem sp, const RelResult &rr)
{
    TRACE1("stemTable" << sp << " := " << rr);
    std::pair<TMap::iterator, bool> inres = table.insert(
	TMap::value_type(sp, rr));
    if (!inres.second) {
        throw std::logic_error("repeated insert");
    }
}

}
