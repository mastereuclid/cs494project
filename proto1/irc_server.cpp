#include "irc_server.hpp"
using std::string, std::move;
using guard = std::lock_guard<std::mutex>;
static std::mutex nickex;
static std::map<const std::string, irc_conn> nicks;
// static std::mutex chantex;
static std::map<const std::string, channel> channels;
static std::mutex limbex;
static std::set<irc_conn> limbo;
const static std::map<std::string, std::function<void(const irc_msg &)>>
    on_event = create_event_map();
const event_t create_event_map() { return event_t{}; }

void insert_limbo(irc_conn &&conn) {
  guard lock(limbex);
  limbo.insert(move(conn));
}

uint limbo_count() {
  guard lock(limbex);
  return limbo.size();
}

void insert_nick(string nick, irc_conn &&conn) {
  guard lock(nickex);
  if (nicks.count(nick) > 0) {
    throw nick_taken();
  }
  nicks.emplace(nick, move(conn));
}
irc_conn::irc_conn(connection &&con) : connection(move(con)) {
  t = std::thread(&irc_conn::engine, this);
}
void irc_conn::engine() {
  // user needs to get out of limbo
}
void irc_conn::close() {
  connection::close();
  running = false;
}
void irc_conn::operator()(void) {
  // I think this should be the thing listening for this user.
}
irc_server::irc_server() : sock(port) {}
irc_server::irc_server(string _port) : port(_port), sock(_port) {}
bool irc_server::nick_exists(string nick) const {
  if (nicks.count(nick) > 0)
    return true;
  return false;
}
void irc_server::start_accepting_clients() {
  // I could launch a lambda
  // I could launch a memberfunction
  // I could use this thread of execution to just keep accepting clients
  auto acceptor = [](bool &listening, server_socket &sock) {
    sock.listen();
    while (listening) {
      insert_limbo(irc_conn(sock.accept()));
    }
  };
  listen_thread = std::thread(acceptor, std::ref(listening), std::ref(sock));
  listen_thread.detach();
}
void irc_server::stop_accepting_clients() {
  listening = false; // I could possibly connect to myself to get past the
                     // blocking accpet call
}

irc_server::~irc_server() {
  if (listen_thread.joinable())
    listen_thread.join();
}
