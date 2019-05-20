#define CATCH_CONFIG_MAIN
#include "socket.hpp"
#include <catch2/catch.hpp>
#include <string>
TEST_CASE("try to connect to waiting server") {
  auto msg = std::string("test from client to server");
  socket client;
  client.connect("localhost", "3030");
  std::string recvd = client.receive();
  client.send(msg);
  REQUIRE(recvd == std::string("test messge from server to client"));
}
