#include "protocol.hpp"
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
  bool running = true;
  std::string port = "6667";
  std::thread accept_thread;
  void engine();
  socket listsock;
};
class nick : public protocol {
public:
  nick(::socket &&);
  // bool operator<(const nick &other) { return nickname < other.nickname; }

  std::string nickname;
  std::string user;
  std::string real;

private:
};
class channel {
  // shouuld a channel have a list of strings and lookup
  // each nick object as needed or should nicks be shared_ptrs
};

} // namespace irc::server
