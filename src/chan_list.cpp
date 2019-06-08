#include "chan_list.hpp"
#include <ncurses.h>
chanlist::chanlist()
    : list(nc::points_t(0, 0, getmaxy(stdscr) - 4, 11), nc::border_t(false, false, false, true)) {}
nicklist::nicklist()
    : list(nc::points_t(0, getmaxx(stdscr) * (9.0 / 10.0), getmaxy(stdscr) - 4, getmaxx(stdscr)),
           nc::border_t(true, false, false, false)) {}