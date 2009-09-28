#include "program.hh"
#include "answer.hh"
#include "trace.hh"
#include "xdoc.hh"
#include "xutil.hh"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

xmlNodePtr new_node(const char *name);

static xmlNodePtr remove_single_node(xmlNodePtr n);

static xmlNodePtr do_prune(xmlNodePtr tree, const Answer &answer);
static void prune(xmlNodePtr tree, const Answer &answer);

int Program::run(const char *tailname, int argc, char **argv)
{
    int rv = -1;

    if (argc != 3) {
        TRACE1("sefira-" << tailname << " usage: " << argv[0] << " from_file to_file");
	return rv;
    }

    LIBXML_TEST_VERSION
    xmlKeepBlanksDefault(0);

    try {
        print_lcs(argv[1], argv[2]);
	rv = 0;
    } catch (std::exception &x) {
        TRACE1("sefira-" << tailname << ": " << x.what());
    }
    catch (...) {
        TRACE1("sefira-" << tailname << ": unknown error!");
    }

    xmlCleanupParser();

    return rv;
}

void Program::print_lcs(const char *fname1, const char *fname2)
{
    XDoc doc1 = xutil::parse_file(fname1);
    XDoc doc2 = xutil::parse_file(fname2);

    bool second;
    Answer a = get_lcs(xutil::get_root_element(doc1),
	xutil::get_root_element(doc2),
	second);

    xmlDocPtr doc = second ? (xmlDocPtr)doc2 : (xmlDocPtr)doc1;
    xmlNodePtr tree = xutil::get_root_element(doc);

    char buffer[6];
    sprintf(buffer, "%d", a.get_score());
    
    xmlNodePtr nt = new_node("lcs");
    xmlDocSetRootElement(doc, nt);
    xutil::append_child(nt, tree);
    xmlSetProp(nt,
	reinterpret_cast<const xmlChar *>("score"),
	reinterpret_cast<const xmlChar *>(buffer));

    a.insert(nt);
    prune(nt, a);

    xmlChar *mem = 0;
    int size = 0;
    xmlDocDumpFormatMemory(doc, &mem, &size, 1);
    
    std::copy(reinterpret_cast<char *>(mem),
	reinterpret_cast<char *>(mem + size),
	std::ostream_iterator<char>(std::cout));

    xmlFree(mem);
}

xmlNodePtr new_node(const char *name)
{
    xmlNodePtr n = xmlNewNode(0,
	reinterpret_cast<const xmlChar *>(name));
    if (!n)
    {
        std::string s = "cannot create ";
	s += name;
	throw std::runtime_error(s);
    }

    return n;
}

static xmlNodePtr remove_single_node(xmlNodePtr n)
{
    if (!n)
    {
	throw std::invalid_argument("no node to prune");
    }

    if (!n->parent)
    {
	throw std::invalid_argument("root removal not supported");
    }

    if (n->children)
    {
        if (!n->last) {
	    throw std::logic_error("libxml2 child pointers inconsistent");
	}

	xmlNodePtr ch = n->children;
	while (ch) {
	    ch->parent = n->parent;
	    ch = ch->next;
	}

	if (n->children->prev) {
	    throw std::logic_error("libxml2 first child not first");
	}

	n->children->prev = n->prev;

	if (n->last->next) {
	    throw std::logic_error("libxml2 last child not last");
	}

	n->last->next = n->next;
    }

    if (n->prev)
    {
        n->prev->next = n->children ? n->children : n->next;
    }

    if (n->next)
    {
        n->next->prev = n->last ? n->last : n->prev;
    }

    if (n == n->parent->children)
    {
	n->parent->children = n->children ? n->children : n->next;
    }

    if (n == n->parent->last)
    {
	n->parent->last = n->last ? n->last : n->prev;
    }

    xmlNodePtr next = n->children ? n->children : n->next;

    n->prev = 0;
    n->next = 0;
    n->parent = 0;
    n->children = 0;
    n->last = 0;

    xmlFree(n);
    return next;
}

static xmlNodePtr do_prune(xmlNodePtr tree, const Answer &answer)
{
    xmlNodePtr next;
    if (!answer.has_node(tree))
    {
        next = remove_single_node(tree);
    }
    else
    {
	xmlNodePtr ch = tree->children;
	while (ch) {
	    ch = do_prune(ch, answer);
	}

	next = tree->next;
    }

    return next;
}

static void prune(xmlNodePtr tree, const Answer &answer)
{
    xmlNodePtr next = do_prune(tree, answer);
    if (next) {
        throw std::logic_error("prune does only trees - not forrests");
    }
}
