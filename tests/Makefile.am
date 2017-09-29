
AM_CPPFLAGS=-I../src/ $(UUID_CFLAGS) $(OPENSSL_CFLAGS) $(JSON_CFLAGS)
LDADD=../src/.libs/libclientserver.a -lcap -lpthread -lrt $(UUID_LIBS) $(OPENSSL_LIBS) $(JSON_LIBS)
CFLAGS=@CHECK_CFLAGS@

TESTS=PIDFile \
	Queueing \
	StringCheck \
	StringChomp \
	StringJoinList \
	StringJoinMap \
	StringJoinVector \
	StringSplitList \
	StringSplitMap \
	StringSplitOne \
	StringSplitVector \
	TestClientConnection \
	TestClient \
	TestClientEvents \
	TestFileExists \
	TestFileRead \
	TestFileSync \
	TestFileWrite \
	TestDirSync \
	TestNotify \
	TestPoller \
	TestProcess \
	TestSelector \
	TestServer \
	TestServerDisconnects \
	TestSignalHandler \
	TestTimers \
	ThreadPool \
	TimerExit \
	TimerExitLamba \
	TestSemaphore \
	TestUuid \
	TestSHA1 \
	TestHashMutex \
	TestTimeNanoSeconds \
	LazyScopedPtr \
	Optional \
	TestUDPMCast \
	TestUDPMCast

check_PROGRAMS=PIDFile \
	Queueing \
	StringCheck \
	StringChomp \
	StringJoinList \
	StringJoinMap \
	StringJoinVector \
	StringSplitList \
	StringSplitMap \
	StringSplitOne \
	StringSplitVector \
	TestClientConnection \
	TestClient \
	TestClientEvents \
	TestFileExists \
	TestFileRead \
	TestFileSync \
	TestFileWrite \
	TestDirSync \
	TestNotify \
	TestPoller \
	TestProcess \
	TestSelector \
	TestServer \
	TestServerDisconnects \
	TestSignalHandler \
	TestTimers \
	ThreadPool \
	TimerExit \
	TimerExitLamba \
	TestSemaphore \
	TestUuid \
	TestSHA1 \
	TestHashMutex \
	TestTimeNanoSeconds \
	LazyScopedPtr \
	Optional \
	TestUDPMCast \
	TestUDPMCastClient

PIDFile_SOURCES=PIDFile.cpp
Queueing_SOURCES=Queueing.cpp
StringCheck_SOURCES=StringCheck.cpp
StringChomp_SOURCES=StringChomp.cpp
StringJoinList_SOURCES=StringJoinList.cpp
StringJoinMap_SOURCES=StringJoinMap.cpp
StringJoinVector_SOURCES=StringJoinVector.cpp
StringSplitList_SOURCES=StringSplitList.cpp
StringSplitMap_SOURCES=StringSplitMap.cpp
StringSplitOne_SOURCES=StringSplitOne.cpp
StringSplitVector_SOURCES=StringSplitVector.cpp
TestClientConnection_SOURCES=TestClientConnection.cpp
TestClient_SOURCES=TestClient.cpp
TestClientEvents_SOURCES=TestClientEvents.cpp
TestFileExists_SOURCES=TestFileExists.cpp
TestFileRead_SOURCES=TestFileRead.cpp
TestFileSync_SOURCES=TestFileSync.cpp
TestFileWrite_SOURCES=TestFileWrite.cpp
TestDirSync_SOURCES=TestDirSync.cpp
TestNotify_SOURCES=TestNotify.cpp
TestPoller_SOURCES=TestPoller.cpp
TestProcess_SOURCES=TestProcess.cpp
TestSelector_SOURCES=TestSelector.cpp
TestServer_SOURCES=TestServer.cpp
TestServerDisconnects_SOURCES=TestServerDisconnects.cpp
TestSignalHandler_SOURCES=TestSignalHandler.cpp
TestTimers_SOURCES=TestTimers.cpp
ThreadPool_SOURCES=ThreadPool.cpp
TimerExit_SOURCES=TimerExit.cpp
TimerExitLamba_SOURCES=TimerExitLamba.cpp
TestSemaphore_SOURCES=TestSemaphore.cpp
TestUuid_SOURCES=TestUuid.cpp
TestSHA1_SOURCES=TestSHA1.cpp
TestHashMutex_SOURCES=TestHashMutex.cpp
TestTimeNanoSeconds_SOURCES=TestTimeNanoSeconds.cpp
LazyScopedPtr_SOURCES=LazyScopedPtr.cpp
Optional_SOURCES=Optional.cpp
TestUDPMCast_SOURCES=TestUDPMCast.cpp
TestUDPMCastClient_SOURCES=TestUDPMCastClient.cpp

include_HEADERS=TestSetup.h TestServer.h

