#include "protocol.hpp"
#include <catch2/catch.hpp>

TEST_CASE("class exists", "[protocol]") {
  std::string firstmsg("NICK albert");
  std::string secondmsg("USER username hostname servername :al B");
  std::string inputmock(
      "NICK albert\r\nUSER username hostname servername :al B\r\n");
  std::string slice1("NICK al");
  std::string slice2("bert\r\nUSER username h");
  std::string slice3("ostname servername :al B\r\n");
  irc::protocol test;
  test.packet(slice1);
  test.packet(slice2);
  test.packet(slice3);
  // REQUIRE(test.parse_next_packet() == firstmsg);
  // REQUIRE(test.get_storage() ==
  //         std::string("USER username hostname servername :al B\r\n"));
  // REQUIRE(test.parse_next_packet() == secondmsg);
  // REQUIRE(test.get_storage().empty());
  auto msptr = test.get_next_msg();
  REQUIRE(msptr->command() == std::string("NICK"));
}
