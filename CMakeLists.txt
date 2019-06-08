cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project("cs494 client server project" CXX)
add_compile_options(-W -Wall -Wextra)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(Catch2 REQUIRED)
find_package (Threads REQUIRED)
find_package(Curses REQUIRED)
# find_package(Qt5Widgets CONFIG REQUIRED)
# SET(CODE_COVERAGE ON)
include(code-coverage.cmake)
SET(DOT_OUTPUT_TYPE png)
include(dependency-graph.cmake)
# SET(IWYU ON)
# include(tools.cmake)
# add_code_coverage()
# add_code_coverage_all_targets()
include(CTest)
include(Catch)
include_directories(headers) #I know this isn't cool anymore....

add_library(catchmain tests/tester.cpp)
add_library(socket src/socket.cpp)
add_library(irc_server src/irc_server.cpp)
target_link_libraries(irc_server PUBLIC ${CMAKE_THREAD_LIBS_INIT} PRIVATE irc_msg_parser PUBLIC socket PRIVATE protocol PUBLIC responses)
target_compile_features(irc_server PUBLIC cxx_std_17)
#target_link_libraries(irc_server irc_msg_parser)


add_executable(socket_tests tests/socket_tests.cpp)
target_link_libraries(socket_tests PRIVATE socket PRIVATE ${CMAKE_THREAD_LIBS_INIT} PRIVATE catchmain)
SET(TEST_APPS ${TEST_APPS} socket_tests)


add_library(irc_msg_parser src/irc_msg.cpp)
# add_code_coverage(irc_msg_parser)

add_executable(parser_test tests/irc_msg_test.cpp)
target_link_libraries(parser_test PRIVATE catchmain PRIVATE irc_msg_parser)
SET(TEST_APPS ${TEST_APPS} parser_test)

add_executable(irc_server_test tests/server_test.cpp)
target_link_libraries(irc_server_test PRIVATE irc_msg_parser PRIVATE socket PRIVATE irc_server PRIVATE catchmain PUBLIC ${CMAKE_THREAD_LIBS_INIT} PRIVATE responses)
SET(TEST_APPS ${TEST_APPS} irc_server_test)

add_library(parser src/parser.cpp)
target_link_libraries(parser PRIVATE irc_msg PRIVATE socket)

add_library(protocol src/protocol.cpp)
target_link_libraries(protocol PRIVATE socket PRIVATE irc_msg_parser PRIVATE queue)

add_executable(proto_test tests/protocol_test.cpp)
target_link_libraries(proto_test PRIVATE protocol PRIVATE catchmain PRIVATE ${CMAKE_THREAD_LIBS_INIT})
SET(TEST_APPS ${TEST_APPS} proto_test)

add_library(queue src/queue.cpp)
target_compile_features(queue PUBLIC cxx_std_17)
add_executable(queue_test tests/queue_test.cpp)
target_link_libraries(queue_test PRIVATE queue PRIVATE catchmain)
SET(TEST_APPS ${TEST_APPS} queue_test)

add_library(responses src/responses.cpp)
target_compile_features(responses PUBLIC cxx_std_17)
add_executable(replies_test tests/response_test.cpp)
target_link_libraries(replies_test PRIVATE catchmain PRIVATE responses)

add_executable(server src/server.cpp)
target_link_libraries(server PRIVATE irc_server)
add_executable(simple_hello tests/client_stub.cpp)
target_link_libraries(simple_hello PRIVATE socket)

add_library(ncwindow src/ncwindow.cpp)
target_link_libraries(ncwindow PUBLIC ${CURSES_LIBRARIES})
target_include_directories(ncwindow PUBLIC ${CURSES_INCLUDE_DIR})

add_executable(window_test tests/window_test.cpp)
target_link_libraries(window_test PUBLIC catchmain PUBLIC ncwindow)
set(TEST_APPS ${TEST_APPS} window_test)

add_library(nclist src/nclist.cpp)
target_link_libraries(nclist PUBLIC ncwindow)

add_executable(nlist_test tests/nclist_test.cpp)
target_link_libraries(nlist_test PUBLIC nclist PUBLIC catchmain)
set(TEST_APPS ${TEST_APPS} nlist_test)

add_executable(irc_client src/client.cpp)
target_link_libraries(irc_client PRIVATE protocol PUBLIC ${CURSES_LIBRARIES})
target_include_directories(irc_client PUBLIC ${CURSES_INCLUDE_DIR})

add_library(chanlist src/chan_list.cpp)
target_link_libraries(chanlist nclist)

add_executable(chanlisttest tests/chanlisttest.cpp)
target_link_libraries(chanlisttest PUBLIC catchmain PUBLIC chanlist)

foreach( tester ${TEST_APPS} )
  catch_discover_tests(${tester})
endforeach()
# catch_discover_tests(parser_test)
