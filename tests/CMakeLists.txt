
add_executable(LazyScopedPtr LazyScopedPtr.cpp)
target_link_libraries(LazyScopedPtr clientserver)
add_test(LazyScopedPtr "./LazyScopedPtr")

add_executable(Optional Optional.cpp)
target_link_libraries(Optional clientserver)
add_test(Optional "./Optional")

add_executable(PIDFile PIDFile.cpp)
target_link_libraries(PIDFile clientserver)
add_test(PIDFile "./PIDFile")

add_executable(Queueing Queueing.cpp)
target_link_libraries(Queueing clientserver)
add_test(Queueing "./Queueing")

add_executable(StringCheck StringCheck.cpp)
target_link_libraries(StringCheck clientserver)
add_test(StringCheck "./StringCheck")

add_executable(StringChomp StringChomp.cpp)
target_link_libraries(StringChomp clientserver)
add_test(StringChomp "./StringChomp")

add_executable(StringJoinList StringJoinList.cpp)
target_link_libraries(StringJoinList clientserver)
add_test(StringJoinList "./StringJoinList")

add_executable(StringJoinMap StringJoinMap.cpp)
target_link_libraries(StringJoinMap clientserver)
add_test(StringJoinMap "./StringJoinMap")

add_executable(StringJoinVector StringJoinVector.cpp)
target_link_libraries(StringJoinVector clientserver)
add_test(StringJoinVector "./StringJoinVector")

add_executable(StringRot13 StringRot13.cpp)
target_link_libraries(StringRot13 clientserver)
add_test(StringRot13 "./StringRot13")

add_executable(StringSplitList StringSplitList.cpp)
target_link_libraries(StringSplitList clientserver)
add_test(StringSplitList "./StringSplitList")

add_executable(StringSplitMap StringSplitMap.cpp)
target_link_libraries(StringSplitMap clientserver)
add_test(StringSplitMap "./StringSplitMap")

add_executable(StringSplitOne StringSplitOne.cpp)
target_link_libraries(StringSplitOne clientserver)
add_test(StringSplitOne "./StringSplitOne")

add_executable(StringSplitVector StringSplitVector.cpp)
target_link_libraries(StringSplitVector clientserver)
add_test(StringSplitVector "./StringSplitVector")

add_executable(TaskPool TaskPool.cpp)
target_link_libraries(TaskPool clientserver)
add_test(TaskPool "./TaskPool")

add_executable(TestClientConnection TestClientConnection.cpp)
target_link_libraries(TestClientConnection clientserver)
add_test(TestClientConnection "./TestClientConnection")

add_executable(TestClient TestClient.cpp)
target_link_libraries(TestClient clientserver)
add_test(TestClient "./TestClient")

add_executable(TestClientEvents TestClientEvents.cpp)
target_link_libraries(TestClientEvents clientserver)
add_test(TestClientEvents "./TestClientEvents")

add_executable(TestDirSync TestDirSync.cpp)
target_link_libraries(TestDirSync clientserver)
add_test(TestDirSync "./TestDirSync")

add_executable(TestFileExists TestFileExists.cpp)
target_link_libraries(TestFileExists clientserver)
add_test(TestFileExists "./TestFileExists")

add_executable(TestFileRead TestFileRead.cpp)
target_link_libraries(TestFileRead clientserver)
add_test(TestFileRead "./TestFileRead")

add_executable(TestFileSync TestFileSync.cpp)
target_link_libraries(TestFileSync clientserver)
add_test(TestFileSync "./TestFileSync")

add_executable(TestFileWrite TestFileWrite.cpp)
target_link_libraries(TestFileWrite clientserver)
add_test(TestFileWrite "./TestFileWrite")

add_executable(TestHashMutex TestHashMutex.cpp)
target_link_libraries(TestHashMutex clientserver)
add_test(TestHashMutex "./TestHashMutex")

add_executable(TestNotify TestNotify.cpp)
target_link_libraries(TestNotify clientserver)
add_test(TestNotify "./TestNotify")

add_executable(TestPoller TestPoller.cpp)
target_link_libraries(TestPoller clientserver)
add_test(TestPoller "./TestPoller")

add_executable(TestProcess TestProcess.cpp)
target_link_libraries(TestProcess clientserver)
add_test(TestProcess "./TestProcess")

add_executable(TestSelector TestSelector.cpp)
target_link_libraries(TestSelector clientserver)
add_test(TestSelector "./TestSelector")

add_executable(TestSemaphore TestSemaphore.cpp)
target_link_libraries(TestSemaphore clientserver)
add_test(TestSemaphore "./TestSemaphore")

add_executable(TestServer TestServer.cpp)
target_link_libraries(TestServer clientserver)
add_test(TestServer "./TestServer")

add_executable(TestServerDisconnects TestServerDisconnects.cpp)
target_link_libraries(TestServerDisconnects clientserver)
add_test(TestServerDisconnects "./TestServerDisconnects")

add_executable(TestSHA1 TestSHA1.cpp)
target_link_libraries(TestSHA1 clientserver)
add_test(TestSHA1 "./TestSHA1")

add_executable(TestSignalHandler TestSignalHandler.cpp)
target_link_libraries(TestSignalHandler clientserver)
add_test(TestSignalHandler "./TestSignalHandler")

add_executable(TestThreadFunction TestThreadFunction.cpp)
target_link_libraries(TestThreadFunction clientserver)
add_test(TestThreadFunction "./TestThreadFunction")

add_executable(TestTimeNanoSeconds TestTimeNanoSeconds.cpp)
target_link_libraries(TestTimeNanoSeconds clientserver)
add_test(TestTimeNanoSeconds "./TestTimeNanoSeconds")

add_executable(TestTimers TestTimers.cpp)
target_link_libraries(TestTimers clientserver)
add_test(TestTimers "./TestTimers")

add_executable(TestUDPMCastClient TestUDPMCastClient.cpp)
target_link_libraries(TestUDPMCastClient clientserver)
add_test(TestUDPMCastClient "./TestUDPMCastClient")

add_executable(TestUDPMCast TestUDPMCast.cpp)
target_link_libraries(TestUDPMCast clientserver)
add_test(TestUDPMCast "./TestUDPMCast")

add_executable(TestUuid TestUuid.cpp)
target_link_libraries(TestUuid clientserver)
add_test(TestUuid "./TestUuid")

add_executable(ThreadPool ThreadPool.cpp)
target_link_libraries(ThreadPool clientserver)
add_test(ThreadPool "./ThreadPool")

add_executable(TimerExit TimerExit.cpp)
target_link_libraries(TimerExit clientserver)
add_test(TimerExit "./TimerExit")

add_executable(TimerExitLamba TimerExitLamba.cpp)
target_link_libraries(TimerExitLamba clientserver)
add_test(TimerExitLamba "./TimerExitLamba")
