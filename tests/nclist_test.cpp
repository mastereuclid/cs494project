#include "nclist.hpp"
#include <catch2/catch.hpp>
#include <iostream>
#include <ncurses.h>
class item : public nc::list_item {
public:
  item(std::string d) : data(std::move(d)) {}
  void on_select() override { std::cout << "you have selected " << data << std::endl; }
  const std::string& display_name() const override { return data; }

private:
  std::string data;
};
TEST_CASE("nlist") {
  initscr();
  nc::points_t p;
  p.topleft_y = 0;
  p.topleft_x = 0;
  getmaxyx(stdscr, p.bottomright_y, p.bottomright_x);
  p.bottomright_x /= 2;
  p.bottomright_y /= 2;
  nc::border_t b;
  b.bottom = true;
  b.left = false;
  b.right = true;
  b.top = true;
  nc::list list{p, b};
  auto a = item("hello");
  std::unique_ptr<nc::list_item> bla(new item("hello"));
  list.add_item(std::move(bla));
  std::unique_ptr<nc::list_item> bla2(new item("hello2"));
  list.add_item(std::move(bla2));
  std::unique_ptr<nc::list_item> bla3(new item("hello3"));
  list.add_item(std::move(bla3));
  std::unique_ptr<nc::list_item> bla4(new item("hello4"));
  list.add_item(std::move(bla4));
  list.draw_list();
  // refresh();
  list.on_focus();
}