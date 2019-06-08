#include "outputwin.hpp"
#include <catch2/catch.hpp>
#include <chrono>
#include <ncurses.h>
#include <thread>

TEST_CASE("output window") {
  initscr();
  outputwin output;
  std::string line1("line11111111111111111111111111111111111111");
  std::string line2("line22222222222222222222222222222222222222222222222222222");
  std::string line3("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod "
                    "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, "
                    "quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo "
                    "consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse "
                    "cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non "
                    "proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
  std::string line4("line4");
  output.add_line(line1);
  output.add_line(line2);
  output.add_line(line3);
  output.add_line(line4);
  output.draw_output();
  std::this_thread::sleep_for(std::chrono::seconds(5));
}