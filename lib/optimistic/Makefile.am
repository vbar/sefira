## Process this file with automake to produce Makefile.in

INCLUDES=-I@LIBXML2_INCL_DIR@ -I. -I..

noinst_LTLIBRARIES = liboptimistic.la

liboptimistic_la_SOURCES = builder.cc directory.cc doubleidx.cc graphedge.cc scoreboard.cc srcgraph.cc twiddle.cc weightmemo.cc

optimisticincdir = $(pkgincludedir)/optimistic
optimisticinc_HEADERS = builder.hh bytemap.hh byteset.hh directory.hh doubleidx.hh graphedge.hh scoreboard.hh shortmap.hh srcgraph.hh twiddle.hh weightmemo.hh
