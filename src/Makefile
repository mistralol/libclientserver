
include ../Makefile.Rules

SRCS= 	HexEncode.cpp	\
		Thread.cpp \
		Request.cpp \
		ServerManager.cpp \
		ServerUnix.cpp \
		ServerUnixConnection.cpp \
		ClientBase.cpp \
		ClientUnix.cpp \
		Client.cpp	\
		Errno.cpp

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
	
