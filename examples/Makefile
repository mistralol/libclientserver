

include ../Makefile.Rules


all: SimpleServer

SimpleServer: ../libclientserver.a .depend SimpleServer.cpp
	$(CPP) $(CPPFLAGS) $(INCLUDE) SimpleServer.cpp -o SimpleServer ../libclientserver.a -lboost_thread -lboost_system

.depend: $(SRCS)
	$(MKDEP) $(INCLUDE) SimpleServer.cpp > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif

clean:
	$(RM) SimpleServer
	
	
