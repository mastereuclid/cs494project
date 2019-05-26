#include "responses.hpp"
#include <catch2/catch.hpp>
TEST_CASE("replies work", "[replies]") {
  REQUIRE(err_NOSUCHNICK("test") == string("test :No such nick/channel"));
}