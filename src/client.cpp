#include "client.hpp"
#include "protocol.hpp"
#include <iostream>
#include <memory>
#include <ncurses.h>
#include <sstream>
#include <thread>
void usage(char** argv) { std::cout << "usage: " << argv[0] << " <server name> <nickname>\n"; }

int main(int argc, char** argv) {
  if (argc != 3) {
    usage(argv);
    return 1;
  }
  std::shared_ptr<irc::protocol> conn;
  try {
    bool running = true;
    conn = std::make_shared<irc::protocol>(argv[1]);
    std::stringstream out;
    out << "NICK " << argv[2] << "\r\n"
        << "USER a a a :A\r\n";

    conn->send(out.str());
    // initscr();
    while (running && conn->isopen()) {
      if (conn->msg_queue_empty()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        continue;
      }
      irc_msg msg = conn->get_next_irc_msg();
      std::cout << msg.line << std::endl;
    }
  } catch (const std::exception& e) {
    // endwin();
    std::cout << e.what() << '\n';
  }
}