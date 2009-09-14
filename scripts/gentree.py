#!/usr/bin/python

import getopt
from math import sqrt
import random
import sys
import xml.dom
from xml.dom.ext import Print


def usage():
    print "usage:", sys.argv[0], "-n tree-size [ -s max-label-count ]"


def make_uniform_tag_char(s):
    return chr(ord('a') + random.randint(0, s - 1))


def make_uniform_tag(s):
    cs = []
    while s >= 26:        
        cs.append(make_uniform_tag_char(26))
        s = s // 26
    cs.append(make_uniform_tag_char(s))
    return ''.join(cs)


def make_low_tag_char(n):
    return chr(ord('a') + n - 1)


def make_low_tag(f):
    t = random.random()
    c = f
    n = 1
    while (t < c) and (n <= 1000):
        c = c * f;
        n = n + 1;

    cs = []
    while n >= 26:        
        cs.append(make_low_tag_char(26))
        n = n // 26
    cs.append(make_low_tag_char(n))
    return ''.join(cs)


def create_element(doc, s):
    if s is None:
        t = make_low_tag(0.9)
    else:
        t = make_uniform_tag(s)
    return doc.createElement(t)


def fill_children(doc, root, n, s):
    assert n >= 0, "tree size can't be negative"

    if not n:
        return
    
    mx = int(sqrt(n))
    if n == (mx + 1) * (mx + 1):
        # rounding error - fix it
        mx += 1
        
    nc = random.randint(1, mx)
    lown = n // nc
    surplus = n - lown * nc
    i = 0
    while i < nc:            
        node = create_element(doc, s)
        root.appendChild(node)

        subn = lown - 1
        if surplus:
            if (surplus == (nc - i)) or (random.random() < float(surplus) / (nc - i)):
                subn += 1
                surplus -= 1

        fill_children(doc, node, subn, s)
        i += 1

    
def print_tree(n, s):
    impl = xml.dom.getDOMImplementation(None)
    doc = impl.createDocument(None, None, None)
    root = create_element(doc, s)
    doc.appendChild(root)
    fill_children(doc, root, n - 1, s)
    Print(doc)
    print ''
    
    
def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hn:s:")
    except getopt.GetoptError, err:
        # print help information and exit:
        print str(err) # will print something like "option -a not recognized"
        usage()
        sys.exit(2)
    n = None
    s = None
    for o, a in opts:
        if o == '-h':
            usage()
            sys.exit()
        elif o == '-n':
            n = int(a)
        elif o == '-s':
            s = int(a)
        else:
            assert False, "unhandled option"

    if (n is None) or (n <= 0) or (not(s is None) and (s <= 0)):
        usage()
        sys.exit(2)

    print_tree(n, s)
    
if __name__ == "__main__":
    main()
    

