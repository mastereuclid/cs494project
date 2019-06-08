#include "ncwindow.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace nc;
window::window() : stdscreenwowner(true) {
  initscr();
  getmaxyx(stdscr, p.bottomright_y, p.bottomright_x);
}
window::window(points_t points, border_t border_arg) : myborder(border_arg), p(points) {
  win = newwin(p.bottomright_y - p.topleft_y + 1, p.bottomright_x - p.topleft_x + 1, p.topleft_y,
               p.topleft_x);
  // WINDOW *newwin(int nlines, int ncols, int begin_y, int begin_x);
  getmaxyx(win, max_y, max_x);
  max_x -= 1;
  max_y -= 1;
  drawborder();
  wrefresh(win);
}
WINDOW* window::winptr() const { return win; }
void window::drawborder() const {
  if (myborder.bottom) {
    // draw_horizontal_border(p.bottomright_y);
    draw_horizontal_border(max_y);
  }
  if (myborder.right) {
    draw_vertical_border(max_x);
  }
  if (myborder.top) {
    draw_horizontal_border(0);
  }
  if (myborder.left) {
    draw_vertical_border(0);
  }
}

int window::last_x() const { return max_x; }
int window::last_y() const { return max_y; }
border_t window::bordervalue() const { return myborder; }

void window::draw_horizontal_border(int y) const {
  for (int loop = 0; loop < p.bottomright_x; loop++) {
    wmove(win, y, loop);
    waddch(win, '_');
  }
}
void window::draw_vertical_border(int x) const {
  for (int loop = 0; loop < p.bottomright_y; loop++) {
    wmove(win, loop, x);
    waddch(win, '|');
  }
}

window::~window() {
  delwin(win);
  if (stdscreenwowner) {
    endwin();
  }
}