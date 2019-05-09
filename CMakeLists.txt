#cmake file
cmake_minimum_required(VERSION 3.13)
project("cs494 client server project" CXX)
enable_testing()
include_directories(include)
add_library(client_socket src/client_socket.cpp)
add_executable(client_socket_test tests/socket_test.cpp)
target_link_libraries(client_socket_test client_socket)
add_test(clientsock client_socket_test)
