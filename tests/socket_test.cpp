#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <client_socket.hpp>
TEST_CASE("client socket", "[client]") {
  client_socket sock("localhost", 9911);
}

