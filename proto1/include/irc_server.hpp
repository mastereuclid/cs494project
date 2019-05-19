#include "exceptions.hpp"
#include "irc_msg.hpp"
#include "server_socket.hpp"
#include <condition_variable> // std::condition_variable
#include <functional>
#include <future>
#include <map>
#include <mutex> // std::mutex, std::unique_lock
#include <queue>
#include <set>
#include <thread>
#include <thread> // std::thread
#include <utility>
#include <vector>
class irc_server;
class channel;
// constexpr std::map<std::string,
//                    std::function<void(const irc_server &, const irc_msg &)>>
//     on_event{(std::string("test"), [](const irc_server &, const irc_msg &)
//     {})};
// constexpr std::map<const std::string, const std::string> he;
class irc_conn;
using event_t = std::map<std::string, std::function<void(const irc_msg &)>>;
const event_t create_event_map();
///////////////////////////thread safe data functions//////////////////////////
void insert_limbo(irc_conn &&conn);
void insert_nick(std::string, irc_conn &&conn);
uint limbo_count();
////////////////////////////classes//////////////////////////////////////
class irc_server {

public:
  irc_server();
  irc_server(std::string port);
  ~irc_server();
  irc_server(const irc_server &) = delete;
  // using nickname = std::string;
  bool nick_exists(std::string nick) const;
  void start_accepting_clients();
  void stop_accepting_clients();

private:
  const std::string port = "6667";
  bool listening = true;
  server_socket sock;
  std::thread listen_thread;
  connection accept();
  void limbo(connection &&client);
};
class channel {
public:
private:
  // name, topic, modes, nicks
  const std::string name;
  std::map<const std::string, std::string> nicks;
  std::string topic;
  std::string modes; // I probably want a structure for modes.
};
// needs to be sortable, maybe
class irc_conn : public connection {
public:
  irc_conn(connection &&con);
  irc_conn(connection &&con, std::string nick, std::string user,
           std::string real);
  ~irc_conn() = default;
  irc_conn(irc_conn &&) = default;
  irc_conn(const irc_conn &) = delete;
  const irc_msg recv() const;
  void operator()(void);
  void engine();
  void close();

private:
  std::string nickname;
  std::string username;
  std::string real;
  std::thread t;
  bool running = true;
  void initialize();
};
