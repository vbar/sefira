#include "compare.hh"
#include "nodeindex.hh"
#include "trace.hh"
#include "xdoc.hh"
#include "xutil.hh"
#include <libxml/parser.h>
#include <iomanip>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>

// This would obviously be much simpler in Python, _except_ counting
// the nodes - Python's standard DOM doesn't even seem to have
// ignorable whitespace, much less the same behavior as libxml2...

TNodeIndex node_count = 0;
TNodeIndex leaf_count = 0;

std::set<xmlNodePtr, less_xmlNodePtr> node_labels;

TNodeIndex do_size_tree(xmlNodePtr node, TNodeIndex depth)
{
    TNodeIndex max_depth = ++depth;

    ++node_count;
    node_labels.insert(node);

    xmlNodePtr ch = node->children;
    if (!ch)
    {
	++leaf_count;
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
    std::cout << "tree size = " << node_count << std::endl;
    std::cout << "label count = " << node_labels.size() << std::endl;
    std::cout << "max depth = " << max_depth << std::endl;
    std::cout << "leaf count = " << leaf_count << std::endl;
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
