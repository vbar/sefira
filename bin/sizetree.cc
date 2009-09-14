#include "compare.hh"
#include "nodeindex.hh"
#include "trace.hh"
#include "xdoc.hh"
#include "xutil.hh"
#include <libxml/parser.h>
#include <iomanip>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

// This would obviously be much simpler in Python, _except_ counting
// the nodes - Python's standard DOM doesn't even seem to have
// ignorable whitespace, much less the same behavior as libxml2...

typedef std::map<xmlNodePtr, TNodeIndex, less_xmlNodePtr> TOccMap;

TNodeIndex nodeCount = 0;
TNodeIndex leafCount = 0;
TOccMap nodeOcc;

TNodeIndex do_size_tree(xmlNodePtr node, TNodeIndex depth)
{
    TNodeIndex max_depth = ++depth;

    ++nodeCount;

    std::pair<TOccMap::iterator, bool> inres = nodeOcc.insert(
        TOccMap::value_type(node, 1));
    if (!inres.second) {
	++(inres.first->second);
    }

    xmlNodePtr ch = node->children;
    if (!ch)
    {
	++leafCount;
    }

    while (ch)
    {
        TNodeIndex d = do_size_tree(ch, depth);
	if (d > max_depth)
	{
	    max_depth = d;
	}

	ch = ch->next;
    }

    return max_depth;
}

void size_tree(const char *fname)
{
    XDoc doc = xutil::parse_file(fname);
    TNodeIndex max_depth = do_size_tree(xutil::get_root_element(doc), 0);

    TNodeIndex mn = static_cast<TNodeIndex>(nodeOcc.size());
    TNodeIndex mx = 0;
    for (TOccMap::const_iterator i = nodeOcc.begin();
	 i != nodeOcc.end();
	 ++i) {
        TNodeIndex occ = i->second;
	if (occ < mn) {
	    mn = occ;
	}

	if (occ > mx) {
	    mx = occ;
	}
    }

    std::cout << "tree size = " << nodeCount << std::endl;
    std::cout << "label count = " << nodeOcc.size() << std::endl;
    std::cout << "min label occurence = " << mn << std::endl;
    std::cout << "max label occurence = " << mx << std::endl;
    std::cout << "max depth = " << max_depth << std::endl;
    std::cout << "leaf count = " << leafCount << std::endl;
}

int main(int argc, char **argv)
{
    int rv = -1;

    if (argc != 2) {
        TRACE1("sizetree usage: " << argv[0] << " doc.xml");
	return rv;
    }

    LIBXML_TEST_VERSION
    xmlKeepBlanksDefault(0);

    try {
        size_tree(argv[1]);
	rv = 0;
    } catch (std::exception &x) {
        TRACE1("sizetree: " << x.what());
    }
    catch (...) {
        TRACE1("sizetree: unknown error!");
    }

    xmlCleanupParser();

    return rv;
}
