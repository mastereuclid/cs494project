#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <client_socket.hpp>
TEST_CASE("client socket exists", "[client]") {
  client_socket sock("localhost", "9911");
}

TEST_CASE("socket throws on bad hostname", "[client]") {
  REQUIRE_THROWS(client_socket("somebadhostname", "9911"));
}