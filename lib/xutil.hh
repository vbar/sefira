// based on dom.c from XML::LibXML, which is Copyright (c) 2001
// Christian Glahn

#ifndef xutil_hh
#define xutil_hh

#include <string>
#include <libxml/tree.h>

class XDoc;

namespace xutil {

xmlNodePtr get_root_element(xmlDocPtr doc);

XDoc parse_file(const char *fname);

std::string get_node_name(xmlNodePtr n);

// A wrapper around xmlGetProp/xmlGetNsProp. Must be passed a valid
// attribute (belonging to some node). Throws on error (never returns
// NULL); the returned value must be freed (by xmlFree).
xmlChar *get_value(xmlAttrPtr a);

void append_child(xmlNodePtr ex_parent, xmlNodePtr new_child);

void remove_child(xmlNodePtr parent, xmlNodePtr child);

void remove_children(xmlNodePtr n);

}

#endif

