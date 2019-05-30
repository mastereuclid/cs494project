#include "irc_msg.hpp"
#include "message.hpp"
#include "protocol.hpp"
#include "responses.hpp"
#include <atomic>
#include <condition_variable>
#include <exception>
#include <map>
#include <mutex>
#include <set>
#include <thread>
#include <unordered_map>
namespace irc::server {
class server {
public:
  void start_accepting_clients();
  void stop_accepting_clients();
  ~server();
  server();
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
  ~nick();
  // bool operator<(const nick &other) { return nickname < other.nickname; }
  void setnickname(std::string newnick);
  void setrealname(std::string newreal);
  void setusername(std::string newuser);
  const std::string &getnickname() const;
  const std::string &getrealname() const;
  const std::string &getusername() const;
  void privmsg(const std::string &from, const std::string &data) const;
  void add_channel(std::string chan_name);
  const std::set<std::string> &list_of_channels() const;
  void quit(const std::string &quitmsg);

private:
  std::string quitmsg = "goodbye forever";
  std::string nickname;
  std::string user;
  std::string real;
  std::set<std::string> channels;
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
  void quit(const std::string &nickname, const std::string &msg);
  void who(std::shared_ptr<nick> user) const;

private:
  void remove_nick(std::string nickname);
  void broadcast(std::string msg) const;
  void broadcast_except(std::shared_ptr<nick> user, std::string msg) const;
  std::string topic = "default topic";
  std::unordered_map<std::string, std::shared_ptr<nick>> list_of_nicks;
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
