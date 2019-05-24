#include "socket.hpp"
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

private:
  bool running = true;
  std::string port = "6667";
  std::thread accept_thread;
  void engine();
  socket listsock;
};
class nick : public socket {

public:
  nick(socket &&, std::string, std::string, std::string);
  // bool operator<(const nick &other) { return nickname < other.nickname; }

private:
  std::string nickname;
  std::string user;
  std::string real;
};
class channel {
  // shouuld a channel have a list of strings and lookup
  // each nick object as needed or should nicks be shared_ptrs
};
class pend_con : public socket {
public:
  bool operator<(const pend_con &other) const {
    return socket::sockfd() < other.sockfd();
  }
  pend_con(socket &&sock);
  pend_con(pend_con &&) = delete;
  pend_con(const pend_con &) = delete;
  ~pend_con();

private:
  std::thread communication_thread;
  void engine();
  std::string nick;
  std::string user;
  std::string realname;
};
} // namespace irc::server
