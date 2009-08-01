#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.50)
AC_INIT(sefira, 0.01, vbar@comp.cz)
AC_CONFIG_SRCDIR([bin/optimistic.cc])
AM_INIT_AUTOMAKE(sefira, 0.01)

AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CXX
AM_PROG_LIBTOOL

# Checks for libraries.
AC_CHECK_LIB([xml2], [xmlCleanupParser])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([string.h])

AC_CHECK_FILE(/usr/include/libxml2/libxml/tree.h,
LIBXML2_INCL_DIR_DEFAULT=/usr/include/libxml2,
LIBXML2_INCL_DIR_DEFAULT=/usr/local/include/libxml2)

AC_ARG_WITH(libxml2-incldir,
[  --with-libxml2-incldir      libxml2 library include files],
LIBXML2_INCL_DIR=$with_libxml2_incldir,
LIBXML2_INCL_DIR=$LIBXML2_INCL_DIR_DEFAULT)
AC_SUBST(LIBXML2_INCL_DIR)

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE

AC_CONFIG_FILES([Makefile
 	bin/Makefile
 	lib/Makefile
 	lib/optimistic/Makefile
 	lib/straight/Makefile
 	lib/systematic/Makefile
	sefira.test/Makefile
	testdata/Makefile
	testdata/optimistic/Makefile
	testdata/straight/Makefile
	testdata/systematic/Makefile])
AC_OUTPUT
