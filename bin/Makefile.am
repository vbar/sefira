##process this file with automake to produce Makefile.in

INCLUDES=-I@LIBXML2_INCL_DIR@ -I../lib

bin_PROGRAMS = sefira-optimistic sefira-straight sefira-systematic

sefira_optimistic_SOURCES = optimistic.cc optprog.cc optprog.hh

sefira_optimistic_LDADD = ../lib/libsefira.la -lxml2 -lstdc++

sefira_straight_SOURCES = straight.cc strprog.cc strprog.hh

sefira_straight_LDADD = ../lib/libsefira.la -lxml2 -lstdc++

sefira_systematic_SOURCES = systematic.cc sysprog.cc sysprog.hh

sefira_systematic_LDADD = ../lib/libsefira.la -lxml2 -lstdc++
