cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)
project("cs494 client server project" CXX)
add_compile_options(-W -Wall -Wextra)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(Catch2 REQUIRED)
find_package (Threads REQUIRED)
include(CTest)
include(Catch)
include_directories(headers) #I know this isn't cool anymore....

add_library(socket src/socket.cpp)

add_library(irc_server src/irc_server.cpp)

add_executable(socket_tests tests/socket_tests.cpp)
target_link_libraries(socket_tests Catch2::Catch2)
target_link_libraries(socket_tests socket)
target_link_libraries(socket_tests ${CMAKE_THREAD_LIBS_INIT})

add_executable(comp_test tests/comp_test.cpp)
target_link_libraries(comp_test Catch2::Catch2)
target_link_libraries(comp_test socket)

catch_discover_tests(socket_tests)
