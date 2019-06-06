#include "ncwindow.hpp"
#include <catch2/catch.hpp>
#include <chrono>
#include <ncurses.h>
#include <thread>
class testwin : public nc::window {
  void on_focus() override {}
  // void un_focus() override {}
public:
  testwin(nc::points_t t, nc::border_t b) : nc::window(t, b) {}
};
TEST_CASE("window exists", "[ui]") {
  initscr();
  nc::points_t p;
  p.topleft_y = 0;
  p.topleft_x = 0;
  getmaxyx(stdscr, p.bottomright_y, p.bottomright_x);
  p.bottomright_x /= 2;
  p.bottomright_y /= 2;
  nc::border_t b;
  b.bottom = true;
  b.left = true;
  b.right = true;
  b.top = true;

  testwin mywin{p, b};

  std::this_thread::sleep_for(std::chrono::seconds(3));
  endwin();
}