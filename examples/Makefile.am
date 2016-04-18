
AM_CPPFLAGS=-I../src/ $(UUID_CFLAGS) $(JSON_CFLAGS)
LDADD=../src/.libs/libclientserver.a $(JSON_LIBS) -lcap -lpthread -lrt

bin_PROGRAMS=SimpleClient SimpleServer PollerListen


SimpleClient_SOURCES=SimpleClient.cpp

SimpleServer_SOURCES=SimpleServer.cpp

PollerListen_SOURCES=PollerListen.cpp

