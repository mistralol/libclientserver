

include ../Makefile.Rules

LIBS+=-lpthread -lrt

all: SimpleServer SimpleClient

SimpleServer: ../libclientserver.a .depend SimpleServer.cpp
	$(CPP) $(CPPFLAGS) $(INCLUDE) SimpleServer.cpp -o SimpleServer ../libclientserver.a $(LIBS)

SimpleClient: ../libclientserver.a .depend SimpleClient.cpp
	$(CPP) $(CPPFLAGS) $(INCLUDE) SimpleClient.cpp -o SimpleClient ../libclientserver.a $(LIBS)

.depend: $(SRCS)
	$(MKDEP) $(INCLUDE) SimpleServer.cpp > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif

clean:
	$(RM) SimpleServer SimpleClient
	
	
