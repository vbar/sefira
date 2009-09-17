##process this file with automake to produce Makefile.in

INCLUDES=-I@LIBXML2_INCL_DIR@ -I../lib

bin_PROGRAMS = sefira-optimistic sefira-plastic sefira-straight sefira-systematic

noinst_PROGRAMS = sizetree

sefira_optimistic_SOURCES = optimistic.cc

sefira_optimistic_LDADD = ../lib/libsefira.la -lxml2 -lstdc++

sefira_plastic_SOURCES = plastic.cc

sefira_plastic_LDADD = ../lib/libsefira.la -lxml2 -lstdc++

sefira_straight_SOURCES = straight.cc

sefira_straight_LDADD = ../lib/libsefira.la -lxml2 -lstdc++

sefira_systematic_SOURCES = systematic.cc

sefira_systematic_LDADD = ../lib/libsefira.la -lxml2 -lstdc++

sizetree_SOURCES = sizetree.cc

sizetree_LDADD = ../lib/libsefira.la -lxml2 -lstdc++
