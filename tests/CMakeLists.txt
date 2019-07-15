
add_executable(LazyScopedPtr LazyScopedPtr.cpp)
target_link_libraries(LazyScopedPtr clientserver)
target_include_directories(LazyScopedPtr PRIVATE .)
add_test(LazyScopedPtr "./LazyScopedPtr")

add_executable(Optional Optional.cpp)
target_link_libraries(Optional clientserver)
target_include_directories(Optional PRIVATE .)
add_test(Optional "./Optional")

add_executable(PIDFile PIDFile.cpp)
target_link_libraries(PIDFile clientserver)
target_include_directories(PIDFile PRIVATE .)
add_test(PIDFile "./PIDFile")

add_executable(Queueing Queueing.cpp)
target_link_libraries(Queueing clientserver)
target_include_directories(Queueing PRIVATE .)
add_test(Queueing "./Queueing")

add_executable(StringCheck StringCheck.cpp)
target_link_libraries(StringCheck clientserver)
target_include_directories(StringCheck PRIVATE .)
add_test(StringCheck "./StringCheck")

add_executable(StringChomp StringChomp.cpp)
target_link_libraries(StringChomp clientserver)
target_include_directories(StringChomp PRIVATE .)
add_test(StringChomp "./StringChomp")

add_executable(StringJoinList StringJoinList.cpp)
target_link_libraries(StringJoinList clientserver)
target_include_directories(StringJoinList PRIVATE .)
add_test(StringJoinList "./StringJoinList")

add_executable(StringJoinMap StringJoinMap.cpp)
target_link_libraries(StringJoinMap clientserver)
target_include_directories(StringJoinMap PRIVATE .)
add_test(StringJoinMap "./StringJoinMap")

add_executable(StringJoinVector StringJoinVector.cpp)
target_link_libraries(StringJoinVector clientserver)
target_include_directories(StringJoinVector PRIVATE .)
add_test(StringJoinVector "./StringJoinVector")

add_executable(StringRot13 StringRot13.cpp)
target_link_libraries(StringRot13 clientserver)
target_include_directories(StringRot13 PRIVATE .)
add_test(StringRot13 "./StringRot13")

add_executable(StringSplitList StringSplitList.cpp)
target_link_libraries(StringSplitList clientserver)
target_include_directories(StringSplitList PRIVATE .)
add_test(StringSplitList "./StringSplitList")

add_executable(StringSplitMap StringSplitMap.cpp)
target_link_libraries(StringSplitMap clientserver)
target_include_directories(StringSplitMap PRIVATE .)
add_test(StringSplitMap "./StringSplitMap")

add_executable(StringSplitOne StringSplitOne.cpp)
target_link_libraries(StringSplitOne clientserver)
target_include_directories(StringSplitOne PRIVATE .)
add_test(StringSplitOne "./StringSplitOne")

add_executable(StringSplitVector StringSplitVector.cpp)
target_link_libraries(StringSplitVector clientserver)
target_include_directories(StringSplitVector PRIVATE .)
add_test(StringSplitVector "./StringSplitVector")

add_executable(TaskPool TaskPool.cpp)
target_link_libraries(TaskPool clientserver)
target_include_directories(TaskPool PRIVATE .)
add_test(TaskPool "./TaskPool")

add_executable(TestClientConnection TestClientConnection.cpp)
target_link_libraries(TestClientConnection clientserver)
target_include_directories(TestClientConnection PRIVATE .)
add_test(TestClientConnection "./TestClientConnection")

add_executable(TestClient TestClient.cpp)
target_link_libraries(TestClient clientserver)
target_include_directories(TestClient PRIVATE .)
add_test(TestClient "./TestClient")

add_executable(TestClientEvents TestClientEvents.cpp)
target_link_libraries(TestClientEvents clientserver)
target_include_directories(TestClientEvents PRIVATE .)
add_test(TestClientEvents "./TestClientEvents")

add_executable(TestDirSync TestDirSync.cpp)
target_link_libraries(TestDirSync clientserver)
target_include_directories(TestDirSync PRIVATE .)
add_test(TestDirSync "./TestDirSync")

add_executable(TestFileExists TestFileExists.cpp)
target_link_libraries(TestFileExists clientserver)
target_include_directories(TestFileExists PRIVATE .)
add_test(TestFileExists "./TestFileExists")

add_executable(TestFileRead TestFileRead.cpp)
target_link_libraries(TestFileRead clientserver)
target_include_directories(TestFileRead PRIVATE .)
add_test(TestFileRead "./TestFileRead")

add_executable(TestFileSync TestFileSync.cpp)
target_link_libraries(TestFileSync clientserver)
target_include_directories(TestFileSync PRIVATE .)
add_test(TestFileSync "./TestFileSync")

add_executable(TestFileWrite TestFileWrite.cpp)
target_link_libraries(TestFileWrite clientserver)
target_include_directories(TestFileWrite PRIVATE .)
add_test(TestFileWrite "./TestFileWrite")

add_executable(TestHashMutex TestHashMutex.cpp)
target_link_libraries(TestHashMutex clientserver)
target_include_directories(TestHashMutex PRIVATE .)
add_test(TestHashMutex "./TestHashMutex")

add_executable(TestNotify TestNotify.cpp)
target_link_libraries(TestNotify clientserver)
target_include_directories(TestNotify PRIVATE .)
add_test(TestNotify "./TestNotify")

add_executable(TestPoller TestPoller.cpp)
target_link_libraries(TestPoller clientserver)
target_include_directories(TestPoller PRIVATE .)
add_test(TestPoller "./TestPoller")

add_executable(TestProcess TestProcess.cpp)
target_link_libraries(TestProcess clientserver)
target_include_directories(TestProcess PRIVATE .)
add_test(TestProcess "./TestProcess")

add_executable(TestSelector TestSelector.cpp)
target_link_libraries(TestSelector clientserver)
target_include_directories(TestSelector PRIVATE .)
add_test(TestSelector "./TestSelector")

add_executable(TestSemaphore TestSemaphore.cpp)
target_link_libraries(TestSemaphore clientserver)
target_include_directories(TestSemaphore PRIVATE .)
add_test(TestSemaphore "./TestSemaphore")

add_executable(TestServer TestServer.cpp)
target_link_libraries(TestServer clientserver)
target_include_directories(TestServer PRIVATE .)
add_test(TestServer "./TestServer")

add_executable(TestServerDisconnects TestServerDisconnects.cpp)
target_link_libraries(TestServerDisconnects clientserver)
target_include_directories(TestServerDisconnects PRIVATE .)
add_test(TestServerDisconnects "./TestServerDisconnects")

add_executable(TestSHA1 TestSHA1.cpp)
target_link_libraries(TestSHA1 clientserver)
target_include_directories(TestSHA1 PRIVATE .)
add_test(TestSHA1 "./TestSHA1")

add_executable(TestSignalHandler TestSignalHandler.cpp)
target_link_libraries(TestSignalHandler clientserver)
target_include_directories(TestSignalHandler PRIVATE .)
add_test(TestSignalHandler "./TestSignalHandler")

add_executable(TestThreadFunction TestThreadFunction.cpp)
target_link_libraries(TestThreadFunction clientserver)
target_include_directories(TestThreadFunction PRIVATE .)
add_test(TestThreadFunction "./TestThreadFunction")

add_executable(TestTimeNanoSeconds TestTimeNanoSeconds.cpp)
target_link_libraries(TestTimeNanoSeconds clientserver)
target_include_directories(TestTimeNanoSeconds PRIVATE .)
add_test(TestTimeNanoSeconds "./TestTimeNanoSeconds")

add_executable(TestTimers TestTimers.cpp)
target_link_libraries(TestTimers clientserver)
target_include_directories(TestTimers PRIVATE .)
add_test(TestTimers "./TestTimers")

add_executable(TestUDPMCastClient TestUDPMCastClient.cpp)
target_link_libraries(TestUDPMCastClient clientserver)
target_include_directories(TestUDPMCastClient PRIVATE .)
add_test(TestUDPMCastClient "./TestUDPMCastClient")

add_executable(TestUDPMCast TestUDPMCast.cpp)
target_link_libraries(TestUDPMCast clientserver)
target_include_directories(TestUDPMCast PRIVATE .)
add_test(TestUDPMCast "./TestUDPMCast")

add_executable(TestUuid TestUuid.cpp)
target_link_libraries(TestUuid clientserver)
target_include_directories(TestUuid PRIVATE .)
add_test(TestUuid "./TestUuid")

add_executable(ThreadPool ThreadPool.cpp)
target_link_libraries(ThreadPool clientserver)
target_include_directories(ThreadPool PRIVATE .)
add_test(ThreadPool "./ThreadPool")

add_executable(TimerExit TimerExit.cpp)
target_link_libraries(TimerExit clientserver)
target_include_directories(TimerExit PRIVATE .)
add_test(TimerExit "./TimerExit")

add_executable(TimerExitLamba TimerExitLamba.cpp)
target_link_libraries(TimerExitLamba clientserver)
target_include_directories(TimerExitLamba PRIVATE .)
add_test(TimerExitLamba "./TimerExitLamba")

