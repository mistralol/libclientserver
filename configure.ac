AC_INIT(libclientserver, 1.0.2, james@stev.org)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([src/Barrier.cpp])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_FILES([libclientserver.pc])

LT_INIT
AM_INIT_AUTOMAKE([foreign])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL

m4_ifdef([AX_COMPILER_FLAGS],
        [
                AX_COMPILER_FLAGS([WARN_CFLAGS],[WARN_LDFLAGS])
                AC_SUBST([AM_CXXFLAGS], $WARN_CXXFLAGS)
        ]
,
        [
                AC_SUBST([AM_CXXFLAGS], ['-Wall -Werror'])
        ]
)

AX_CXX_COMPILE_STDCXX_11(noext, optional)

dnl Check for libcap
PKG_CHECK_MODULES(CAP, [
        libcap >= 0.6.6
], [
        AC_SUBST(CAP_CFLAGS)
        AC_SUBST(CAP_LIBS)
], [
        AC_MSG_ERROR([
                You need to install or upgrade libcap-dev
        ])
])

dnl Check for libuuid
PKG_CHECK_MODULES(UUID, [
        uuid >= 2.25.0
], [
        AC_SUBST(UUID_CFLAGS)
        AC_SUBST(UUID_LIBS)
], [
        AC_MSG_ERROR([
                You need to install or upgrade uuid-dev
        ])
])

dnl Check for openssl
PKG_CHECK_MODULES(OPENSSL, [
        openssl >= 1.0.1
], [
        AC_SUBST(OPENSSL_CFLAGS)
        AC_SUBST(OPENSSL_LIBS)
], [
        AC_MSG_ERROR([
                You need to install or upgrade openssl or libssl-dev
        ])
])

dnl Check for jsoncpp
PKG_CHECK_MODULES(JSON, [
	jsoncpp >= 0.6.0
], [
	AC_SUBST(JSON_CFLAGS)
	AC_SUBST(JSON_LIBS)
], [
	AC_MSG_ERROR([
		You need to install or upgrade jsoncpp
	])
])

nodedir="/usr/lib/node_modules"
AC_SUBST(nodedir)

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


AC_OUTPUT(Makefile src/Makefile node/Makefile examples/Makefile tests/Makefile docs/Makefile)

