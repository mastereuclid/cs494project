#include "ncwindow.hpp"
#include <functional>
#include <string>
class inputwin : nc::window {
public:
  inputwin();
  void on_focus() override;
  void get_input(std::function<void(std::string)>) const;

private:
  mutable std::string buffer;
  void clear() const;
};