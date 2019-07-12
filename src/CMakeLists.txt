
set(CLIENTSERVER_SOURCES
    Barrier.cpp
    Buffer.cpp
    Caps.cpp
    ClientBase.cpp
    Client.cpp
    ClientTCP.cpp
    ClientUnix.cpp
    ClientUnixSelectedConnection.cpp
    ClientUnixSelected.cpp
    Crypto.cpp
    DirUtil.cpp
    Errno.cpp
    Exception.cpp
    FileUtil.cpp
    GroupID.cpp
    IServer.cpp
    Mutex.cpp
    Notify.cpp
    PerfCounter.cpp
    PerfManager.cpp
    PIDFile.cpp
    Poller.cpp
    Process.cpp
    RateLimit.cpp
    RequestMap.cpp
    RWLock.cpp
    ScopedLock.cpp
    ScopedReadLock.cpp
    ScopedSignalBlock.cpp
    ScopedWriteLock.cpp
    Selector.cpp
    Semaphore.cpp
    ServerException.cpp
    ServerManager.cpp
    ServerPolledConnection.cpp
    ServerSelectedConnection.cpp
    ServerTCP6PolledListener.cpp
    ServerTCP6SelectedListener.cpp
    ServerTCPPolled.cpp
    ServerTCPPolledListener.cpp
    ServerTCPSelected.cpp
    ServerTCPSelectedListener.cpp
    ServerUnixConnection.cpp
    ServerUnix.cpp
    ServerUnixPolled.cpp
    ServerUnixPolledListener.cpp
    ServerUnixSelected.cpp
    ServerUnixSelectedListener.cpp
    SetGid.cpp
    SetUid.cpp
    SignalHandler.cpp
    String.cpp
    Thread.cpp
    ThreadFunction.cpp
    ThreadPool.cpp
    ThreadPoolThread.cpp
    Time.cpp
    TimerAbort.cpp
    TimerFunc.cpp
    Timers.cpp
    UDPMCastClient.cpp
    UDPMCast.cpp
    UserID.cpp
    Uuid.cpp
)

add_library(clientserver SHARED ${CLIENTSERVER_SOURCES})
add_library(clientserverstatic STATIC ${CLIENTSERVER_SOURCES})
set_target_properties(clientserverstatic PROPERTIES OUTPUT_NAME clientserver)

target_include_directories(clientserver PUBLIC
    .
    ${UUID_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIRS}
    ${CAP_INCLUDE_DIRS}
)
target_include_directories(clientserverstatic PUBLIC
    .
    ${UUID_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIRS}
    ${CAP_INCLUDE_DIRS}
)
target_link_libraries(clientserver PUBLIC
    -lpthread -lrt
    ${UUID_LIBRARIES}
    ${JSONCPP_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${CAP_LIBRARIES}
)
target_link_libraries(clientserverstatic PUBLIC
    -lpthread -lrt
    ${UUID_LIBRARIES}
    ${JSONCPP_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${CAP_LIBRARIES}
)

install(TARGETS clientserver clientserverstatic
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(FILES libclientserver.h
    DESTINATION include
)

install(DIRECTORY libclientserver
    DESTINATION include
)
