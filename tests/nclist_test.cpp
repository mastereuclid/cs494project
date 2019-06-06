#include "nclist.hpp"
#include <catch2/catch.hpp>
#include <ncurses.h>
TEST_CASE("nlist") {
  initscr();
  nc::list list;
  refresh();
  getch();
}