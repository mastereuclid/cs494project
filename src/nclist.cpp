#include "nclist.hpp"
using namespace nc;
#include <chrono>
#include <iostream>
#include <thread>
list::list(points_t points, border_t border) : window(points, border) {}

void list::on_focus() {
  // capture input until a give-up focus button is pressed
  cbreak();
  noecho();
  keypad(winptr(), true);
  for (int input = wgetch(winptr()); input != button_tab; input = wgetch(winptr())) {
    switch (input) {
    case KEY_UP:
      selector_up();
      break;
    case KEY_DOWN:
      selector_down();
      break;
    case button_enter:
      items.at(selected % items.size())->on_select();
      break;
    default:
      std::cout << "|" << input << "| pressed\n";
      break;
    }
    draw_list();
    wrefresh(winptr());
    // refresh();
  }
  std::cout << "tab pressed\n";
  std::this_thread::sleep_for(std::chrono::seconds(2));
  // highlight the currently selected item in the list
  // capture up and down arrows and move the selected item
}
void list::draw_list() const {
  // is the list empty, smaller than the screen, or larger than the screen
  auto line = 0;
  for (auto row = start; row <= last; row++) {
    auto& item = items.at(row);
    std::string output;
    if (row == selected) {
      output.append(">");
      output.append(item->display_name());
    } else {
      output.append(item->display_name());
    }
    if (int64_t diff = last_x() - output.size(); diff > 0) {
      output.append(diff - 1, ' ');
    }

    // wmove(winptr(), line, 1);
    wmove(winptr(), line, bordervalue().left ? 1 : 0);
    waddstr(winptr(), output.c_str());
    line++;
  }
  // wrefresh(winptr());
}

void list::add_item(std::shared_ptr<list_item> item) {
  items.push_back(std::move(item));
  if (items.size() < last_y()) { // NOLINT still makes sense with different signs
    last = items.size() - 1;
  }
  // draw_list();
}

void list::remove_item(const std::string& name) {
  for (auto item = items.begin(); item != items.end(); item++) {
    if ((*item)->display_name() == name)
      items.erase(item);
    break;
  } // not efficient
}

void list::selector_down() {
  // naive test
  selected++;
  selected %= items.size();
}
void list::selector_up() {
  // naive implementation
  selected--;
  selected %= items.size();
}
