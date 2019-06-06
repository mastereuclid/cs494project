#pragma once
#include "ncwindow.hpp"
#include <memory>
#include <ncurses.h>
#include <string>
#include <vector>
namespace nc {
// <template typename item>
class list_item {
public:
  virtual const std::string& display_name() const = 0;
  virtual void on_select() = 0;
  virtual ~list_item() = default;

private:
};

// this class will assume inistr; has been called

class list : public window {
public:
  // window(points_t points, border_t border);
  list(points_t points, border_t border);
  void on_focus() override;
  // I need a way to add items
  void add_item(std::unique_ptr<list_item> item);
  void remove_item(const std::string& name);
  void draw_list() const;

private:
  std::vector<std::unique_ptr<list_item>> items;
  std::vector<std::unique_ptr<list_item>>::const_iterator selected =
      items.cbegin();
};
} // namespace nc