#include "channel_item.hpp"
#include <sstream>
channel_item::channel_item(std::string name, std::shared_ptr<irc::protocol> conn)
    : chan_name(std::move(name)), conn(conn) {}

channel_item::~channel_item() {}

void channel_item::on_select() {
  nicks.draw_list();
  text.on_focus();
  input.on_focus();
  input.get_input([this](std::string line) { process_input(line); });
}
void channel_item::add_line(std::string line) { text.add_line(line); }

void channel_item::process_input(std::string line) {
  // if line starts with a slash its a command
  if (line.size() == 0) {
    return;
  }
  text.add_line(line);
  if (line.at(0) == '/') {
    text.add_line(line);
    // process as a command
    auto space = line.find_first_of(' ');
    // space--;
    auto command = line.substr(0, space);
    // std::stringstream temp;
    // text.add_line(temp.str());
    // temp << "command:" << command;
    if (command == std::string("/join")) {
      text.add_line("join command");
      // omg how am I going to handle this...
      std::stringstream out;
      out << "JOIN " << line.substr(space + 1);
      conn->sendircmsg(out.str());
    } else if (command == std::string("/part")) {
      // ugh
      std::stringstream out;
      out << "PART " << line.substr(space + 1);
      conn->sendircmsg(out.str());
    } else if (command == std::string("/quit")) {
      // quiting...
      std::stringstream out;
      out << "QUIT " << line.substr(space + 1);
      conn->sendircmsg(out.str());
    }
  } else {
    send(line);
  }
}

void channel_item::send(std::string line) {
  std::stringstream out;
  out << "PRIVMSG " << chan_name << " :" << line;
  conn->sendircmsg(out.str());
}