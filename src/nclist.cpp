#include "nclist.hpp"
using namespace nc;

list::list(points_t points, border_t border) : window(points, border) {}

void list::on_focus() {
  // capture input until a give-up focus button is pressed
  for (int input = getch(); input != KEY_BTAB; input = getch()) {
  }
  // highlight the currently selected item in the list
  // capture up and down arrows and move the selected item
}
void list::draw_list() const {}