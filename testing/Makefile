

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
		StringChomp.cpp			\
		StringCheck.cpp			\
		PIDFile.cpp				\
		Queueing.cpp			\
		TestServer.cpp			\
		TestClient.cpp			\
		TestClientConnection.cpp \
		TestClientEvents.cpp	\
		TestNotify.cpp \
		TestTimers.cpp \
		TimerExit.cpp \
		TestProcess.cpp \
		ThreadPool.cpp \
		TestFileRead.cpp \
		TestFileWrite.cpp \
		TestFileSync.cpp \
		TestFileExists.cpp \
		TestSignalHandler.cpp


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
	

