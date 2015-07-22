AC_INIT(libclientserver, 1.0.0, james@stev.org)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([src/Barrier.cpp])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_FILES([libclientserver.pc])

LT_INIT
AM_INIT_AUTOMAKE([foreign])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL

AC_SUBST([AM_CXXFLAGS], ["-Wall -Werror"])


AC_CHECK_HEADER_STDBOOL()

AC_TYPE_SIZE_T
AC_TYPE_UINT64_T

AC_CHECK_FUNCS([localtime_r])


AC_ARG_ENABLE([docs], AS_HELP_STRING([--enable-docs], [Enable docs to be built using doxygen]))
AS_IF([test "x$enable_docs" = "xyes"], [
  AC_CHECK_PROGS([DOXYGEN], [doxygen])
  if test -z "$DOXYGEN";
     then AC_MSG_ERROR([Doxygen requested but not found])
  fi
])


AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([docs/Doxyfile])])


AC_OUTPUT(Makefile src/Makefile examples/Makefile tests/Makefile docs/Makefile)

