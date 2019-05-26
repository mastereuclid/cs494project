#include "protocol.hpp"
#include <condition_variable>
#include <exception>
#include <mutex>
#include <thread>
namespace irc::server {
class server {
public:
  void start_accepting_clients();
  void stop_accepting_clients();
  ~server();
  server() = default;
  server(server &&) = delete;
  server(const server &) = delete;
  uint conn_count() const;
  uint limbo_count() const;

private:
  bool running = false;
  std::string port = "6667";
  std::thread accept_thread;
  void engine();
  socket listsock;
  std::mutex awake;
  std::condition_variable server_up;
};
class nick : public protocol {
public:
  nick(::socket &&);
  // bool operator<(const nick &other) { return nickname < other.nickname; }
  void setnickname(std::string newnick);
  void setrealname(std::string newreal);
  void setusername(std::string newuser);
  const std::string &getnickname() const;
  const std::string &getrealname() const;
  const std::string &getusername() const;

private:
  std::string nickname;
  std::string user;
  std::string real;
};
class channel {
  // shouuld a channel have a list of strings and lookup
  // each nick object as needed or should nicks be shared_ptrs
};
class nick_in_use : public std::exception {
public:
  const char *what() const noexcept override {
    return "nickname is already taken";
  }
};
} // namespace irc::server
