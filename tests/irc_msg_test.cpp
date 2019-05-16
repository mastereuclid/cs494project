#define CATCH_CONFIG_MAIN
#include "irc_msg.hpp"
#include <catch2/catch.hpp>
using std::string;
TEST_CASE("irc_msg exists") { irc_msg(); }

TEST_CASE("parse a privmsg", "[parse]") {
  std::string line(
      string(":euclid_!~euclid@c-67-189-121-140.hsd1.or.comcast.net ") +
      string("PRIVMSG euclid_ :hello there I'm euclid3"));
  irc_msg msg(std::move(line));
  REQUIRE(msg.from_nick() == std::string("euclid_"));
  CHECK(msg.from_user() == string("~euclid"));
  CHECK(msg.from_host() == string("c-67-189-121-140.hsd1.or.comcast.net"));
  CHECK(msg.from_type() == irc_msg::prefix_type::USER);
  CHECK(msg.command() == string("PRIVMSG"));
}

TEST_CASE("parse a nick change", "[parse]") {}

TEST_CASE("who command", "[parse]") {
  string line("WHO #my_own_test_channel");
  irc_msg msg(std::move(line));
  REQUIRE(msg.from_type() == irc_msg::prefix_type::NONE);
  REQUIRE(msg.command() == string("WHO"));
  REQUIRE(msg.params() == string("#my_own_test_channel"));
}

//:irc.Prison.NET PONG irc.Prison.NET :LAG1557902350392289
TEST_CASE("prefix for servername works", "[parse]") {

  irc_msg msg(
      string(":irc.Prison.NET PONG irc.Prison.NET :LAG1557902350392289"));
  REQUIRE(msg.from_type() == irc_msg::prefix_type::SERVERNAME);
  REQUIRE(msg.command() == string("PONG"));
  REQUIRE(msg.params() == string("irc.Prison.NET"));
  REQUIRE(msg.data() == string("LAG1557902350392289"));
}
