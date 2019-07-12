

add_executable(PollerListen PollerListen.cpp)
target_link_libraries(PollerListen clientserver ${JSONCPP_LIBRARIES})

add_executable(SimpleClient SimpleClient.cpp)
target_link_libraries(SimpleClient clientserver ${JSONCPP_LIBRARIES})

add_executable(SimpleServer SimpleServer.cpp)
target_link_libraries(SimpleServer clientserver ${JSONCPP_LIBRARIES})

