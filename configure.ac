AC_INIT(liblogger, 1.0.0, james@stev.org)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([src/Barrier.cpp])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_FILES([libclientserver.pc])

LT_INIT
AM_INIT_AUTOMAKE([foreign])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL

AC_SUBST([AM_CXXFLAGS], [-std=c++11])

AC_OUTPUT(Makefile src/Makefile examples/Makefile tests/Makefile)


AC_CHECK_HEADER_STDBOOL()


AC_TYPE_SIZE_T
AC_TYPE_UINT64_T

AC_CHECK_FUNCS([localtime_r])

