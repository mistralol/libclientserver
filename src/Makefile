
include ../Makefile.Rules

SRCS= 	Thread.cpp \
		Request.cpp \
		RequestMap.cpp \
		ServerManager.cpp \
		ServerException.cpp \
		IServer.cpp \
		ServerUnix.cpp \
		ServerUnixConnection.cpp \
		ClientBase.cpp \
		ClientUnix.cpp \
		Client.cpp	\
		Errno.cpp \
		String.cpp \
		Time.cpp \
		Mutex.cpp \
		ScopedLock.cpp \
		RWLock.cpp \
		ScopedReadLock.cpp \
		ScopedWriteLock.cpp \
		ReadBuffer.cpp \
		Encoder.cpp \
		Decoder.cpp \
		PIDFile.cpp \
		PerfCounter.cpp \
		PerfManager.cpp \
		Notify.cpp \
		Barrier.cpp \
		Timers.cpp \
		TimerAbort.cpp

OBJS=$(SRCS:%.cpp=%.o)


all: ../libclientserver.a ../libclientserver.so

../libclientserver.a: $(OBJS) .depend
	$(AR) cru ../libclientserver.a $(OBJS)

../libclientserver.so: $(OBJS) .depend
	$(CPP) -shared -o ../libclientserver.so $(OBJS)

.cpp.o:
	$(CPP) $(CPPFLAGS) $(INCLUDE) -c $< -o $@

.depend: $(SRCS)
	$(MKDEP) $(INCLUDE) $(SRCS) > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif

clean:
	$(RM) $(OBJS) .depend
	$(RM) ../libclientserver.a ../libclientserver.so
	
