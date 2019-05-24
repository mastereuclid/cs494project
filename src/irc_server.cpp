#include "irc_server.hpp"
#include <atomic>
#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
#include <set>
#include <unordered_map>
using namespace irc::server;
using guard = std::lock_guard<std::mutex>;
using nickptr = std::shared_ptr<nick>;
//////////////// data structures  //////////////////
// nicks
std::mutex nickex;
static std::unordered_map<std::string, nickptr> nicks;
// channels
std::mutex chanex;
static std::unordered_map<std::string, channel> channels;
std::atomic<uint> num_in_limbo = 0;
// pending connections

///////////////// thread safe data function //////////////////

// bool insert_nick(socket &&sock, std::string nickname, std::string user,
//                  std::string real) {
//   guard lock(nickex);
//   if (nicks.count(nickname) > 0)
//     return false;
//   auto newnick = std::make_shared<nick>(std::move(sock));
//   nicks.emplace(nickname, newnick);
//   return true;
// }

void limbo(socket &&sock) {
  try {
    // I need to limit the attempts and time
    num_in_limbo++;
    std::cout << "connection in limbo" << std::endl;
    std::unique_lock<std::mutex> mylock(nickex, std::defer_lock);
    std::string nick;
    std::string user;
    std::string realname;
    auto time_limit =
        std::chrono::system_clock::now() + std::chrono::seconds(10);
    for (uint attempts = 0;
         attempts < 4 && std::chrono::system_clock::now() < time_limit;
         attempts++) {
      // check for incoming

      std::string msg = sock.receive();
      std::cout << msg << std::endl;
      irc_msg line(std::move(msg));
      // std::cout << line.command() << *line.middleparam().begin() <<
      // std::endl;
      if (line.command() == std::string("NICK")) {
        nick = *line.middleparam().begin();
      } else if (line.command() == std::string("USER")) {
        user = *line.middleparam().begin();
        realname = line.data();
      }
      if (!nick.empty() && !user.empty() && !realname.empty()) {
        mylock.lock();
        if (nicks.count(nick) > 0) {
          std::cout << "nick taken" << std::endl;
        } else {
          // insert in nicks
          std::cout << "nick accepted" << std::endl;
        }
        mylock.unlock();
      }
    }
  } catch (const std::exception &e) {
    std::cout << "limbo: " << e.what() << std::endl;
  }
  num_in_limbo--;
}

///////////////////class definitionsm threading/////////////////////////////////
void server::start_accepting_clients() {
  accept_thread = std::thread(&server::engine, this);
  std::this_thread::yield();
}
void server::stop_accepting_clients() {
  running = false;
  socket temp;
  try {
    temp.connect("localhost", "6667");
  } catch (const std::exception &e) {
    // do nothing. its fine if the socket fails we just need the engine to
    // unblock
  }
}
server::~server() {
  if (accept_thread.joinable())
    accept_thread.join();
}
nick::nick(::socket &&sock) : irc::protocol(std::move(sock)) {}

uint server::conn_count() const {
  std::lock_guard<std::mutex> lock1(nickex);
  return nicks.size();
}
uint server::limbo_count() const { return num_in_limbo; }

void server::engine() {
  // this is in its own thread so it needs to catch everything
  // std::cout << "omg what is happening\n";
  try {
    listsock.bind(port);
    while (running) {
      socket temp = listsock.accept();
      if (running)
        // insert_into_pending(std::move(client));
        std::thread(limbo, std::move(temp)).detach();
    }
  } catch (const std::exception &e) {

    std::cout << "server engine exception: " << e.what();
  }
}
