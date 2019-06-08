#include "inputwin.hpp"
#include <catch2/catch.hpp>
#include <chrono>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <thread>
TEST_CASE("input") {
  initscr();
  inputwin textwin, textwin2;
  textwin.on_focus();
  std::string a;
  auto handler = [&a](std::string arg) {
    a = arg;
    move(1, 1);
    // std::cout << "text: " << a << '\n';
  };
  textwin.get_input(handler);
  textwin2.on_focus();
  textwin2.get_input(handler);
  textwin.on_focus();
  endwin();
  std::cout << "text: " << a << '\n';

  std::this_thread::sleep_for(std::chrono::seconds(2));
}