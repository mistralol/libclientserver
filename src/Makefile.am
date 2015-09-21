AM_CPPFLAGS=

lib_LTLIBRARIES=libclientserver.la

libclientserver_la_CPPFLAGS=$(UUID_CFLAGS) $(OPENSSL_CFLAGS)
libclientserver_la_LDFLAGS=$(UUID_LIBS) $(OPENSSL_LIBS) -version-info 1:0:2

libclientserver_la_SOURCES= Barrier.cpp \
			    Encoder.cpp \
			    PerfCounter.cpp \
			    Request.cpp \
			    ServerException.cpp \
			    SetGid.cpp \
			    Time.cpp \
			    Uuid.cpp \
			    Buffer.cpp \
			    Errno.cpp \
			    PerfManager.cpp \
			    RequestMap.cpp \
			    ServerManager.cpp \
			    SetUid.cpp \
			    TimerAbort.cpp \
			    Caps.cpp \
			    FileUtil.cpp \
			    DirUtil.cpp \
			    PIDFile.cpp \
			    RWLock.cpp \
			    ServerUnixConnection.cpp \
			    SignalHandler.cpp \
			    TimerFunc.cpp \
			    ClientBase.cpp \
			    GroupID.cpp \
			    Poller.cpp \
			    ScopedLock.cpp \
			    ServerUnix.cpp \
			    String.cpp \
			    Timers.cpp \
			    Client.cpp \
			    IServer.cpp \
			    Process.cpp \
			    ScopedReadLock.cpp \
			    ServerUnixSelectedConnection.cpp \
			    Thread.cpp \
			    UserID.cpp \
			    ClientUnix.cpp \
			    ClientUnixSelected.cpp \
			    ClientUnixSelectedConnection.cpp \
			    Mutex.cpp \
			    Semaphore.cpp \
			    RateLimit.cpp \
			    ScopedWriteLock.cpp \
			    ServerUnixSelected.cpp \
			    ThreadPool.cpp \
			    Decoder.cpp \
			    Notify.cpp \
			    Selector.cpp \
			    ServerUnixSelectedListener.cpp \
			    ThreadPoolThread.cpp \
			    Crypto.cpp


include_HEADERS=libclientserver.h

otherincludedir = $(includedir)/libclientserver

otherinclude_HEADERS= libclientserver/Barrier.h \
			libclientserver/Buffer.h \
			libclientserver/Caps.h \
			libclientserver/ClientBase.h \
			libclientserver/Client.h \
			libclientserver/ClientUnix.h \
			libclientserver/ClientUnixSelected.h \
			libclientserver/ClientUnixSelectedConnection.h \
			libclientserver/Decoder.h \
			libclientserver/Encoder.h \
			libclientserver/Errno.h \
			libclientserver/FileUtil.h \
			libclientserver/DirUtil.h \
			libclientserver/GroupID.h \
			libclientserver/IClientHandler.h \
			libclientserver/IDataType.h \
			libclientserver/INotify.h \
			libclientserver/IPollable.h \
			libclientserver/ISelectable.h \
			libclientserver/IServerConnection.h \
			libclientserver/IServer.h \
			libclientserver/IServerHandler.h \
			libclientserver/ISignalHandler.h \
			libclientserver/ITimer.h \
			libclientserver/Mutex.h \
			libclientserver/Semaphore.h \
			libclientserver/Notify.h \
			libclientserver/PerfCounter.h \
			libclientserver/PerfManager.h \
			libclientserver/PIDFile.h \
			libclientserver/Poller.h \
			libclientserver/Process.h \
			libclientserver/Queue.h \
			libclientserver/SQueue.h \
			libclientserver/RateLimit.h \
			libclientserver/Request.h \
			libclientserver/RequestMapEntry.h \
			libclientserver/RequestMap.h \
			libclientserver/RWLock.h \
			libclientserver/ScopedLock.h \
			libclientserver/ScopedReadLock.h \
			libclientserver/ScopedWriteLock.h \
			libclientserver/Selector.h \
			libclientserver/ServerConnectionBase.h \
			libclientserver/ServerException.h \
			libclientserver/ServerManager.h \
			libclientserver/ServerUnixConnection.h \
			libclientserver/ServerUnix.h \
			libclientserver/ServerUnixSelectedConnection.h \
			libclientserver/ServerUnixSelected.h \
			libclientserver/ServerUnixSelectedListener.h \
			libclientserver/SetGid.h \
			libclientserver/SetUid.h \
			libclientserver/SetUser.h \
			libclientserver/SignalHandler.h \
			libclientserver/String.h \
			libclientserver/Thread.h \
			libclientserver/ThreadPool.h \
			libclientserver/ThreadPoolThread.h \
			libclientserver/Time.h \
			libclientserver/TimerAbort.h \
			libclientserver/TimerFunc.h \
			libclientserver/Timers.h \
			libclientserver/UserID.h \
			libclientserver/Uuid.h \
			libclientserver/Crypto.h



