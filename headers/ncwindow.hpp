#pragma once
#include <memory>
#include <ncurses.h>
#include <string>
#include <vector>
namespace nc {
const int button_tab = 9;
const int button_enter = 10;
enum class measure_t { absolute, percentage };
struct points_t {
  points_t() = default;
  ~points_t() = default;
  points_t(const points_t&) = default;
  points_t(points_t&&) = default;
  points_t(int ay, int ax, int by, int bx)
      : topleft_y(ay), topleft_x(ax), bottomright_y(by), bottomright_x(bx) {}
  int topleft_y = 0;
  int topleft_x = 0;
  int bottomright_y = 0;
  int bottomright_x = 0;
};
struct border_t {
  border_t() = default;
  ~border_t() = default;
  border_t(const border_t&) = default;
  border_t(border_t&&) = default;
  border_t(bool left, bool top, bool bottom, bool right)
      : left(left), top(top), bottom(bottom), right(right) {}
  bool top = true;
  bool bottom = true;
  bool left = true;
  bool right = true;
};
struct measure {
  int value = 0;
  measure_t type = measure_t::absolute;
};
class window {
public:
  window();
  window(points_t points, border_t border);
  ~window();
  virtual void on_focus() = 0;
  // virtual void un_focus() = 0;
protected:
  int last_x() const;
  int last_y() const;
  WINDOW* winptr() const;
  border_t bordervalue() const;

private:
  WINDOW* win = nullptr;
  int max_y = 0, max_x = 0;
  border_t myborder;
  void drawborder() const;
  void draw_horizontal_border(int y) const;
  void draw_vertical_border(int x) const;
  bool stdscreenwowner = false;
  points_t p;
};
} // namespace nc