#pragma once
#include "chan_list.hpp"
#include "inputwin.hpp"
#include "outputwin.hpp"
#include "protocol.hpp"
#include <memory>
#include <string>
class nick_item : public nc::list_item {
public:
  const std::string& display_name() const override { return nickname; }
  void on_select() override {}
  std::string nickname;
};
class channel_item : public nc::list_item {
  channel_item(std::string name, std::shared_ptr<irc::protocol> conn);
  ~channel_item();
  const std::string& display_name() const override { return chan_name; }
  void on_select() override;
  void add_nick();
  void remove_nick();
  void send(std::string);
  void process_input(std::string line);

private:
  // I need an output window and a nicklist
  const std::string chan_name;
  std::shared_ptr<irc::protocol> conn;
  std::string input_string;
  outputwin text;
  inputwin input;
  nicklist nicks;
};