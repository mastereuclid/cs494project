#include "protocol.hpp"
#include <catch2/catch.hpp>

TEST_CASE("one message spread across many packets", "[protocol]") {
  std::string firstmsg("NICK albert");
  std::string secondmsg("USER username hostname servername :al B");
  std::string inputmock(
      "NICK albert\r\nUSER username hostname servername :al B\r\n");
  std::string slice1("NICK al");
  std::string slice2("bert\r\nUSER username h");
  std::string slice3("ostname servername :al B\r\n");
  irc::protocol test;
  test.packet("NICK al");
  test.packet("bert\r\nUSER username h");
  test.packet("ostname servername :al B\r\n");
  // REQUIRE(test.parse_next_packet() == firstmsg);
  // REQUIRE(test.get_storage() ==
  //         std::string("USER username hostname servername :al B\r\n"));
  // REQUIRE(test.parse_next_packet() == secondmsg);
  // REQUIRE(test.get_storage().empty());
  REQUIRE_FALSE(test.msg_queue_empty());
  auto msptr = test.get_next_irc_msg_ptr();
  REQUIRE(msptr->command() == std::string("NICK"));
  msptr = test.get_next_irc_msg_ptr();
  REQUIRE(test.msg_queue_empty());
}

TEST_CASE("multi messages in one line", "[protocol]") {
  irc::protocol test;
  test.packet("NICK albert\r\nUSER username hostname servername :al B\r\n");
  REQUIRE(test.get_next_irc_msg_ptr()->command() == std::string("NICK"));
  REQUIRE(test.get_next_irc_msg_ptr()->command() == std::string("USER"));
}