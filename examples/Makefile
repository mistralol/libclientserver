

include ../Makefile.Rules


all: SimpleServer SimpleClient

SimpleServer: ../libclientserver.a .depend SimpleServer.cpp
	$(CPP) $(CPPFLAGS) $(INCLUDE) SimpleServer.cpp -o SimpleServer ../libclientserver.a -lboost_thread -lboost_system

SimpleClient: ../libclientserver.a .depend SimpleClient.cpp
	$(CPP) $(CPPFLAGS) $(INCLUDE) SimpleClient.cpp -o SimpleClient ../libclientserver.a -lboost_thread -lboost_system

.depend: $(SRCS)
	$(MKDEP) $(INCLUDE) SimpleServer.cpp > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif

clean:
	$(RM) SimpleServer SimpleClient
	
	
