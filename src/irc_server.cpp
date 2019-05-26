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
using msgptr = std::unique_ptr<irc_msg>;
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

void insert_nick(std::string nickname, nickptr newnick) {
  guard lock(nickex);
  if (nicks.count(nickname) > 0) {
    throw nick_in_use();
  }
  nicks.emplace(nickname, newnick);
}

uint nick_count() {
  guard lock(nickex);
  return nicks.size();
}

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
    std::string command_nick("NICK");
    std::string command_user("USER");
    std::string nickname;
    std::string username;
    std::string realname;
    nickptr connection = std::make_shared<irc::server::nick>(std::move(sock));
    // this loop doesn't check a number of attempts, might add later
    for (auto time_limit =
             std::chrono::system_clock::now() + std::chrono::seconds(150);
         std::chrono::system_clock::now() < time_limit;) {
      // check for incoming
      if (connection->msg_queue_empty()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        continue;
      } else {
        while (!connection->msg_queue_empty()) {
          // get all the messages in the queue. See if they are a NICK or USER
          // silently ignore other types or nicks that are already taken
          msgptr msg = connection->get_next_irc_msg_ptr();
          if (msg->command() == command_nick) {
            if (msg->num_of_params() == 1) {
              nickname = msg->middleparam().at(0);
            }
          } else if (msg->command() == command_user) {
            if (msg->num_of_params() > 1) {
              username = msg->middleparam().at(0);
              realname = msg->data();
            }
          }
          if (!nickname.empty() && !username.empty() && !realname.empty()) {
            // std::cout << nickname << " and " << username << std::endl;
            // check if username/nickname combo is available
            // if not should probably unset nickname
            try {
              insert_nick(nickname, connection);
            } catch (const nick_in_use &e) {
            }
            // we successfully inserted a nick and need to set the data in the
            // nick object
            connection->setnickname(nickname);
            connection->setusername(username);
            connection->setrealname(realname);
            // I think an motd function should probably go here
          }
        }
      }
    }
  } catch (const std::exception &e) {
    std::cout << "limbo: " << e.what() << std::endl;
  }
  num_in_limbo--;
}

///////////////////class definitionsm threading/////////////////////////////////
void server::start_accepting_clients() {
  // accept_thread = std::thread(&server::engine, this);
  std::thread(&server::engine, this).detach();
  std::this_thread::yield();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::unique_lock<std::mutex> lock(awake);
  // while (!running)
  // server_up.wait(lock);
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

uint server::conn_count() const { return nick_count(); }
uint server::limbo_count() const { return num_in_limbo; }

void server::engine() {
  // this is in its own thread so it needs to catch everything
  // std::cout << "omg what is happening\n";
  server_up.notify_all();
  try {
    listsock.bind(port);
    running = true;
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

nick::nick(::socket &&sock) : protocol(std::move(sock)) {}
void nick::setnickname(std::string newnick) { nickname = std::move(newnick); }
void nick::setrealname(std::string newreal) { user = std::move(newreal); }
void nick::setusername(std::string newuser) { real = std::move(newuser); }
const std::string &nick::getnickname() const { return nickname; }
const std::string &nick::getrealname() const { return real; }
const std::string &nick::getusername() const { return user; }
