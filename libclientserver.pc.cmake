prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=@CMAKE_INSTALL_PREFIX@
libdir=@CMAKE_INSTALL_PREFIX@/lib
includedir=@CMAKE_INSTALL_PREFIX@/include

Name: libclientserver
Description: Json Client / Server lib for C++
URL: http://github.com/mistralol/libclientserver
Version: @VERSION@
Requires:
Libs: -L${libdir} -lclientserver
Cflags: -I${includedir}
