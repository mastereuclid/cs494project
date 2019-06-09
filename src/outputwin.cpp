#include "outputwin.hpp"
#include <catch2/catch.hpp>
#include <chrono>
#include <ncurses.h>
#include <thread>
outputwin::outputwin()
    : nc::window(nc::points_t(0, 11, getmaxy(stdscr) - 5, getmaxx(stdscr) * (9.0 / 10.0)),
                 nc::border_t(true, false, true, true)) {
  add_line("output window");
}

void outputwin::on_focus() { draw_output(); }
void outputwin::clear() const {
  for (int y = 1; y < last_y(); y++) {
    for (int x = 1; x < last_x(); x++) {
      wmove(winptr(), y, x);
      waddch(winptr(), ' ');
    }
  }
}

void outputwin::draw_output() const {
  clear();
  drawborder();
  auto iter = lines.rbegin();
  for (int pos = last_y() - 1; pos > 0;) {
    if (iter == lines.rend()) {
      break;
    }
    pos -= draw_string(pos, *iter);
    iter++;
  }
  // int nextpos = d;
  wrefresh(winptr());
}

void outputwin::add_line(std::string line) {
  lines.emplace_back(std::move(line));
  if (lines.size() > 30) {
    lines.erase(lines.begin());
  }
  draw_output();
}

int outputwin::draw_string(int y, const std::string& line) const {
  // how much room do I have?
  int len = last_x() - 1;
  int rows = line.size() / len;
  rows += 1;
  wmove(winptr(), y - rows, 1);
  waddstr(winptr(), line.c_str());
  return rows;
}