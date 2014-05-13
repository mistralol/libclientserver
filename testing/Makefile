

include ../Makefile.Rules

LIBS+=../libclientserver.a -lpthread -lrt
DEPS=../libclientserver.a

SRCS=	\
		StringSplitOne.cpp		\
		StringSplitVector.cpp	\
		StringSplitList.cpp		\
		StringSplitMap.cpp		\
		StringJoinVector.cpp	\
		StringJoinList.cpp		\
		StringJoinMap.cpp		\
		PIDFile.cpp				\
		Queueing.cpp			\
		TestServer.cpp			\
		TestClient.cpp			\
		TestClientConnection.cpp \
		TestClientEvents.cpp


OBJS=$(SRCS:%.cpp=%.o)
EXES=$(SRCS:%.cpp=%.exe)

INCLUDE+=-I.


all: $(EXES)

%.o: %.cpp $(DEPS) .depend
	$(CPP) $(CPPFLAGS) $(INCLUDE) -o $@ -c $<

%.exe: %.o
	$(CPP) -o $@ $< $(LIBS)

.depend: $(SRCS_CPP)
	$(MKDEP) $(INCLUDE) $(SRCS) > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif

clean:
	$(RM) $(OBJS) $(EXES)
	

