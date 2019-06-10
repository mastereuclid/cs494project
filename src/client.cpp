#include "client.hpp"
#include "chan_list.hpp"
#include "channel_item.hpp"
#include "protocol.hpp"
#include <atomic>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <ncurses.h>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <vector>
void usage(char** argv) { std::cout << "usage: " << argv[0] << " <server name> <nickname>\n"; }

int main(int argc, char** argv) {
  if (argc != 3) {
    usage(argv);
    return 1;
  }
  std::string me = argv[2];
  std::shared_ptr<irc::protocol> conn;
  std::atomic<bool> running(true);
  try {
    conn = std::make_shared<irc::protocol>(argv[1]);
  } catch (const std::exception& e) {
    // endwin();
    std::cout << e.what() << '\n';
  }
  std::stringstream out;
  out << "NICK " << argv[2] << "\r\n"
      << "USER a a a :A\r\n";

  conn->send(out.str());
  initscr();
  chanlist list;
  std::shared_ptr<nc::list_item> serverwin(new channel_item("server", conn));
  auto engine = [&list, &serverwin, &running, &conn, &me]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::map<const std::string, std::shared_ptr<nc::list_item>> channels;
    while (running && conn->isopen()) {
      if (conn->msg_queue_empty()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        continue;
      }
      irc_msg msg = conn->get_next_irc_msg();
      serverwin->add_line(msg.line);
      if (msg.command() == std::string("JOIN") && msg.from_nick() == me) {
        auto chan = std::shared_ptr<nc::list_item>(new channel_item(msg.data(), conn));
        list.add_item(chan);
        channels.emplace(chan->display_name(), chan);
      } else if (msg.command() == std::string("PART") && msg.from_nick() == me) {
        list.remove_item(msg.middleparam().at(0));

      } else if (msg.command() == std::string("PRIVMSG")) {
        std::stringstream line;
        line << "<" << msg.from_nick() << "> " << msg.data();
        channels.at(msg.middleparam().at(0))->add_line(line.str());
      }
    }
  };
  auto thread = std::thread(engine);
  list.add_item(serverwin);
  list.on_focus();
  list.draw_list();
  running = false;
  serverwin->add_line("test line");
  serverwin->add_line("test line2");
  serverwin->add_line("test line");
  serverwin->add_line("test line2");
  // serverwin->on_select();
  // channel_item serverwin("", conn);

  endwin();
  thread.join();
}