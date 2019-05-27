#include "irc_server.hpp"
#include <thread>
using namespace irc;
int main() {
  server::server serv;
  serv.start_accepting_clients();
  while (true)
    std::this_thread::sleep_for(std::chrono::seconds(2));
  return 0;
}