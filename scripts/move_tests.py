#!/usr/bin/python

from glob import glob
import getopt
import os
import re
import sys


def usage():
    print "usage:", sys.argv[0], "-m minimal-moved-number -d delta"
    print "must be run from the top-level sefira directory"
    

def get_max(stem):
    mask = "testdata/[0-9][0-9]"
    mask += stem
    mask += ".xml"

    filtr = re.compile("[0-9][0-9]")
    
    mx = -1;
    for fname in glob(mask):
        m = filtr.search(fname)
        n = int(m.group())
        if n > mx:
            mx = n

    return mx


def make_path(d, n, stem):
    p = "testdata/"
    if d:
        p += d
        p += "/"

    p += ("%02d" % n)
    p += stem
    p += ".xml"
    return p


def make_command(directory, n, stem, delta):
    cmd = "git mv"
    cmd += " "
    cmd += make_path(directory, n, stem)
    cmd += " "
    cmd += make_path(directory, n + delta, stem)
    return cmd

    
def move_test(n, d):
    cmd = []
    cmd.append(make_command('', n, 'a', d))
    cmd.append(make_command('', n, 'b', d))
    
    for dirpath, dirnames, filenames in os.walk("testdata"):
        for sd in dirnames:
            cmd.append(make_command(sd, n, 'd', d))
            
        # don't go below the first level (there's nothing there
        # anyway)
        break

    for c in cmd:
        r = os.system(c)
        if r:
            print c, "returned", r

        
def move_all(mn, d):
    mx = get_max('a')
    altmx = get_max('b')
    if mx < altmx:
        print "missing test inputs - won't move anything"
        sys.exit(2)
    elif mx > altmx:
        print "test inputs missing - won't move anything"
        sys.exit(2)

    if mn > mx:
        print "no test number", mn, "- won't move anything"
        sys.exit(0)

    while mx >= mn:
        move_test(mx, d)
        mx = mx - 1

    # missing git add here...

    
def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hm:d:")
    except getopt.GetoptError, err:
        # print help information and exit:
        print str(err) # will print something like "option -a not recognized"
        usage()
        sys.exit(2)
    m = None
    d = None
    for o, a in opts:
        if o == '-h':
            usage()
            sys.exit()
        elif o == '-m':
            m = int(a)
        elif o == '-d':
            d = int(a)
        else:
            assert False, "unhandled option"

    if (m is None) or (d is None) or (m < 0) or (d <= 0):
        usage()
        sys.exit(2)

    move_all(m, d)
    
if __name__ == "__main__":
    main()
    

