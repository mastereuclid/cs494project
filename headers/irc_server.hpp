#include "irc_msg.hpp"
#include "protocol.hpp"
#include "responses.hpp"
#include <atomic>
#include <condition_variable>
#include <exception>
#include <mutex>
#include <set>
#include <thread>

namespace irc::server {
class server {
public:
  void start_accepting_clients();
  void stop_accepting_clients();
  ~server();
  server() = default;
  server(std::string name);
  server(server &&) = delete;
  server(const server &) = delete;
  uint conn_count() const;
  uint limbo_count() const;

private:
  std::atomic<bool> running = false;
  std::string port = "6667";
  std::thread msg_thread;
  void engine();
  void msg_engine();
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
  void privmsg(const std::string &from, const std::string &data) const;

private:
  std::string nickname;
  std::string user;
  std::string real;
};
class channel {
public:
  channel() = delete;
  channel(std::string name);
  const std::string chan_name;
  void set_topic(std::string);
  const std::string &get_topic() const;
  void join(std::shared_ptr<nick> user);
  void privmsg(std::shared_ptr<nick> user, const std::string &msg) const;

private:
  void broadcast(std::string msg) const;
  std::string topic = "default topic";
  std::set<std::shared_ptr<nick>> list_of_nicks;
  // modes
  // should a channel have a list of strings and lookup
  // each nick object as needed or should nicks be shared_ptrs
};
class nick_in_use : public std::exception {
public:
  const char *what() const noexcept override {
    return "nickname is already taken";
  }
};
} // namespace irc::server
