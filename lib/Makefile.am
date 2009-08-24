##process this file with automake to produce Makefile.in

SUBDIRS = optimistic straight systematic

INCLUDES=-I@LIBXML2_INCL_DIR@ -I.

lib_LTLIBRARIES = libsefira.la

sefiraincdir = $(pkgincludedir)
sefirainc_HEADERS = app.hh answer.hh compare.hh compareimpl.hh envelope.hh nodeindex.hh program.hh relresult.hh trace.hh xbuffer.hh xdoc.hh xutil.hh treetable.hh

libsefira_la_SOURCES = answer.cc compare.cc program.cc relresult.cc treetable.cc xbuffer.cc xdoc.cc xutil.cc

libsefira_la_LDFLAGS = -rpath $(libdir) -version-info 1:0:0 optimistic/liboptimistic.la straight/libstraight.la systematic/libsystematic.la -lxml2 -lstdc++

