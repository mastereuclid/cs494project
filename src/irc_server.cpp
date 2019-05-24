#include "irc_server.hpp"
#include "irc_msg.hpp"
#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
#include <set>
#include <unordered_map>
using namespace irc::server;
using guard = std::lock_guard<std::mutex>;
using nickptr = std::shared_ptr<nick>;
using pendptr = std::unique_ptr<pend_con>;
//////////////// data structures  //////////////////
// nicks
std::mutex nickex;
static std::unordered_map<std::string, nickptr> nicks;
// channels
std::mutex chanex;
static std::unordered_map<std::string, channel> channels;

// pending connections
std::mutex pendex;
std::set<pendptr> pending;

///////////////// thread safe data function //////////////////

void insert_into_pending(pendptr &&newcon) {
  guard lock(pendex);
  pending.insert(std::move(newcon));
  std::cout << "pending has: " << pending.size() << std::endl;
}
bool insert_nick(socket &&sock, std::string nickname, std::string user,
                 std::string real) {
  guard lock(nickex);
  if (nicks.count(nickname) > 0)
    return false;
  auto newnick =
      std::make_shared<nick>(nick(std::move(sock), nickname, user, real));
  nicks.emplace(nickname, newnick);
  return true;
}

void limbo(socket &&sock) {
  try {
    // I need to limit the attempts and time
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
          std::string reply("443 ");
          reply.append(nick).append("Nickname is already in use");
          // sock.send(reply);
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
nick::nick(socket &&sock, std::string nickname, std::string user,
           std::string real)
    : socket(std::move(sock)), nickname(std::move(nickname)),
      user(std::move(user)), real(std::move(real)) {}

uint server::conn_count() const {
  std::unique_lock<std::mutex> lock1(nickex, std::defer_lock);
  std::unique_lock<std::mutex> lock2(pendex, std::defer_lock);
  std::lock(lock1, lock2);
  return pending.size() + nicks.size();
}

void server::engine() {
  // this is in its own thread so it needs to catch everything
  // std::cout << "omg what is happening\n";
  try {
    listsock.bind(port);
    std::set<pend_con> pend;
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

pend_con::pend_con(socket &&sock) : socket(std::move(sock)) {
  communication_thread = std::thread(&pend_con::engine, this);
}
void pend_con::engine() {
  // own thread catch everything
  try {
    // I need to limit the attempts and time
    auto time_limit =
        std::chrono::system_clock::now() + std::chrono::seconds(10);
    for (uint attempts = 0;
         attempts < 4 && std::chrono::system_clock::now() < time_limit;
         attempts++) {
      // check for incoming
      auto line = irc_msg(receive());
      std::cout << line.command() << *line.middleparam().begin() << std::endl;
      if (line.command() == std::string("NICK")) {
        nick = *line.middleparam().begin();
      } else if (line.command() == std::string("USER")) {
        user = *line.middleparam().begin();
        realname = line.data();
      }
      if (!nick.empty() && !user.empty() && !realname.empty()) {
        // pendptr = extract_pending()
        if (insert_nick(std::move(*this), nick, user, realname)) {

        } else {
          std::string reply("443 ");
          reply.append(nick).append("Nickname is already in use");
          send(reply);
        }
      }
    }
  } catch (const std::exception &e) {
    std::cout << "pend engine: " << e.what() << std::endl;
  }
}
pend_con::~pend_con() {
  if (communication_thread.joinable())
    communication_thread.join();
}
