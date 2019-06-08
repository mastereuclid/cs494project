#include "outputwin.hpp"
#include <catch2/catch.hpp>
#include <chrono>
#include <ncurses.h>
#include <thread>
outputwin::outputwin()
    : nc::window(nc::points_t(0, 12, getmaxy(stdscr) - 5, getmaxx(stdscr) * (9.0 / 10.0) - 1),
                 nc::border_t(true, false, true, true)) {}

void outputwin::on_focus() { draw_output(); }

void outputwin::draw_output() const {
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