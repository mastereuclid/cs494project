#include "chan_list.hpp"
#include <catch2/catch.hpp>
#include <chrono>
#include <ncurses.h>
#include <thread>
TEST_CASE() {
  initscr();
  chanlist list;
  nicklist list2;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  endwin();
}