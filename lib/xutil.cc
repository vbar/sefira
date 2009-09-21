#include "xutil.hh"
#include "xdoc.hh"
#include "xbuffer.hh"
#include <sstream>
#include <stdexcept>
#include <string.h>

using std::string;

static void add_last(xmlNodePtr leader, xmlNodePtr cur)
{
    if (!leader) {
	throw std::invalid_argument("no leader");
    }

    if (!cur) {
	throw std::invalid_argument("no cur");
    }

    if (leader == cur) {
	throw std::invalid_argument("leader cannot be cur");
    }

    if (cur->type == XML_DOCUMENT_FRAG_NODE) {
	throw std::invalid_argument("adding fragments not supported");
    }

    xmlNodePtr p = leader->parent;
    cur->parent = p;
       
    leader->next = cur;
    cur->prev = leader;
	   
    if (p) {
	p->last = cur;
    }
}

static void unlink_node(xmlNodePtr n)
{
    if (!n) {
	throw std::invalid_argument("can't unlink NULL");
    }

    if (n->prev) {
        n->prev->next = n->next;
    }

    if (n->next) {
        n->next->prev = n->prev;
    }

    if (n->parent) {
        if (n == n->parent->last) {
            n->parent->last = n->prev;
        }

        if (n == n->parent->children) {
            n->parent->children = n->next;
        }
    }

    n->prev = 0;
    n->next = 0;
    n->parent = 0;
}

static bool equal_to(xmlNsPtr top, xmlNsPtr ns)
{
    if (!top) {
	throw std::invalid_argument("equal_to: no top");
    }

    if (!(top->prefix)) {
	throw std::invalid_argument("top has no prefix");
    }

    if (!(top->href)) {
	throw std::invalid_argument("top has no href");
    }
    
    if (!ns) {
	throw std::invalid_argument("no ns");
    }

    return ns->prefix &&
	!strcmp(reinterpret_cast<const char *>(top->prefix),
	    reinterpret_cast<const char *>(ns->prefix)) &&
	ns->href &&
	!strcmp(reinterpret_cast<const char *>(top->href),
	    reinterpret_cast<const char *>(ns->href));
}

static void repoint(xmlNsPtr top, xmlNodePtr n)
{
    if (!n) {
	throw std::invalid_argument("repoint: no n");
    }

    if (n->type != XML_ELEMENT_NODE) {
	return;
    }

    if (n->ns && equal_to(top, n->ns)) {
	n->ns = top;
    }

    xmlAttrPtr attr = n->properties;
    while (attr) {
	if (attr->ns && equal_to(top, attr->ns)) {
	    attr->ns = top;
	}

	attr = attr->next;
    }

    xmlNodePtr ch = n->children;
    while (ch) {
	repoint(top, ch);
	ch = ch->next;
    }
}

// Remove the namespace equivalent to top (which must have both local
// name and href and must be declared above n) from the list starting
// at ns_head and return the new head.
static xmlNsPtr purge_one(xmlNsPtr top, xmlNsPtr ns_head)
{
    xmlNsPtr ns = ns_head;
    xmlNsPtr prev = 0;
    while (ns) {
	if (equal_to(top, ns)) {
	    if (prev) {
		prev->next = ns->next;
	    } else {
		ns_head = ns->next;
	    }

	    ns->next = 0; // necessary?

	    xmlFreeNs(ns);

	    return ns_head;
	}

	prev = ns;
	ns = ns->next;
    }

    return ns_head;
}

static void purge(xmlNsPtr top, xmlNodePtr n)
{
    if (n->type != XML_ELEMENT_NODE) {
	return;
    }

    // 7Aug2003: Once is enough: the parser won't allow more than 1
    // equivalent ns declaration in one node.
    n->nsDef = purge_one(top, n->nsDef);

    xmlNodePtr ch = n->children;
    while (ch) {
	purge(top, ch);
	ch = ch->next;
    }
}

xmlNodePtr xutil::get_root_element(xmlDocPtr doc)
{
    if (!doc) {
	throw std::invalid_argument("get_root_element: no doc");
    }

    xmlNodePtr de = xmlDocGetRootElement(doc);
    if (!de) {
	throw std::runtime_error("empty document");
    }

    return de;
}

XDoc xutil::parse_file(const char *fname)
{
    xmlDocPtr doc = xmlParseFile(fname);
    if (!doc) {
	string s = "error parsing ";
	s += fname;
	throw std::runtime_error(s);
    }

    return XDoc(doc);
}

string xutil::get_node_name(xmlNodePtr n)
{
    string out;

    if (n->ns && n->ns->prefix) {
	out = reinterpret_cast<const char *>(n->ns->prefix);
	out += ':';
    }

    if (n->name)
    {
	out += reinterpret_cast<const char *>(n->name);
    }
    else // 21Sep2009: does happen, although perhaps only in a buggy
	 // program...
    {
	out += "<unnamed>";
    }

    return out;
}

xmlChar *xutil::get_value(xmlAttrPtr a)
{
    if (!a) {
	throw std::invalid_argument("get_value: no a");
    }

    if (!(a->parent)) {
	throw std::invalid_argument("attribute has no parent");
    }

    xmlChar *out;

    if (a->ns) {
        if (!(a->ns->href)) {
	    throw std::runtime_error("namespace w/o name");
	}

	out = xmlGetNsProp(a->parent, a->name, a->ns->href);
    } else {
	out = xmlGetProp(a->parent, a->name);
    }

    if (!out) {
	// should be pretty rare, but let's say an allocation might
	// have failed
	throw std::runtime_error("cannot get attribute value");
    }

    return out;
}

void xutil::append_child(xmlNodePtr ex_parent, xmlNodePtr new_child)
{
    if (!ex_parent) {
	throw std::invalid_argument("no ex_parent");
    }

    if (!new_child) {
	throw std::invalid_argument("no new_child");
    }

    if (ex_parent->doc != new_child->doc) {
	throw std::invalid_argument("can't append node from different document");
    }

    unlink_node(new_child);

    if (new_child->type == XML_DOCUMENT_FRAG_NODE) {
	throw std::invalid_argument("fragments not supported");
    }

    if (ex_parent->children) {
        add_last(ex_parent->last, new_child);
    } else {
        ex_parent->children = new_child;
        ex_parent->last = new_child;
        new_child->parent= ex_parent;
    }

    xmlReconciliateNs(ex_parent->doc, new_child);     
}

void xutil::remove_child(xmlNodePtr parent, xmlNodePtr child)
{
    if (!parent) {
	throw std::invalid_argument("remove_child: no parent");
    }

    if (!child) {
	throw std::invalid_argument("remove_child: no child");
    }

    if (child->type == XML_ATTRIBUTE_NODE) {
	throw std::invalid_argument("remove_child: attribute removal not supported");
    }

    if (child->type == XML_NAMESPACE_DECL) {
	throw std::invalid_argument("remove_child: namespace removal not supported");
    }

    if (parent != child->parent) {
        std::stringstream ss;
	ss << "remove_child: " << child << " not child of " << parent;
	throw std::invalid_argument(ss.str());
    }

    unlink_node(child);
}

void xutil::remove_children(xmlNodePtr n)
{
    if (n->children)
    {
	xmlFree(n->children);
	n->children = 0;
	n->last = 0;
    }
}
