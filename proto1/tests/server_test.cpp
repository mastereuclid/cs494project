#define CATCH_CONFIG_MAIN
#include "client_socket.hpp"
#include "irc_server.hpp"
#include <catch2/catch.hpp>
#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>
using std::string;
TEST_CASE("server exists", "[irc_server]") {
  irc_server serv;
  REQUIRE_FALSE(serv.nick_exists(string("euclid")));
}
// TEST_CASE("bla") { client_socket client("localhost", "6667"); }

TEST_CASE("raw connect to server", "[irc_server]") {
  REQUIRE(limbo_count() == 0);
}
