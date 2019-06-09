#include "ncwindow.hpp"
#include <string>
#include <vector>
class outputwin : public nc::window {
public:
  outputwin();
  void on_focus() override;
  void add_msg(std::string nick, std::string message);
  void draw_output() const;
  void add_line(std::string line);

  ~outputwin() = default;

private:
  int draw_string(int y, const std::string& line) const;
  void clear() const;
  // data structure here for the text
  std::vector<std::string> lines;
};