#include "inputwin.hpp"
#include <iostream>
inputwin::inputwin()
    : nc::window(nc::points_t(getmaxy(stdscr) - 5, 0, getmaxy(stdscr), getmaxx(stdscr)),
                 nc::border_t(false, true, false, false)) {}
void inputwin::on_focus() {
  clear();
  echo();
  wmove(winptr(), 1, 0);
}

void inputwin::get_input(std::function<void(std::string)> pass_along) const {
  for (int input = wgetch(winptr()); input != nc::button_tab; input = wgetch(winptr())) {
    if (input == nc::button_enter) {
      clear();
      // do something with buffer
      pass_along(buffer);
      buffer.clear();
      wmove(winptr(), 1, 0);
      continue;
    }
    const char key = static_cast<char>(input);
    buffer.append(1, key);
  }
  buffer.clear();
}

void inputwin::clear() const {
  int max_x = getmaxx(winptr()), max_y = getmaxy(winptr());
  for (int y = 1; y < max_y; y++) {
    for (int x = 0; x < max_x; x++) {
      wmove(winptr(), y, x);
      waddch(winptr(), ' ');
    }
  }
  wmove(winptr(), 1, 0);
  wrefresh(winptr());
}