#pragma once
#include <memory>
#include <ncurses.h>
#include <string>
#include <vector>
namespace nc {
enum class measure_t { absolute, percentage };
struct points_t {
  int topleft_y = 0;
  int topleft_x = 0;
  int bottomright_y = 0;
  int bottomright_x = 0;
};
struct border_t {
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