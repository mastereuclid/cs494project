#pragma once
#include "nclist.hpp"
class chanlist : public nc::list {
public:
  chanlist();
  // void on_focus() override;
};

class nicklist : public nc::list {
public:
  nicklist();
  void on_focus() override {}
};