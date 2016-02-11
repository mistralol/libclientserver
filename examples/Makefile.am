
AM_CPPFLAGS=-I../src/ $(UUID_CFLAGS)
LDADD=../src/.libs/libclientserver.a -lcap -lpthread -lrt

bin_PROGRAMS=SimpleClient SimpleServer PollerListen


SimpleClient_SOURCES=SimpleClient.cpp

SimpleServer_SOURCES=SimpleServer.cpp

PollerListen_SOURCES=PollerListen.cpp

