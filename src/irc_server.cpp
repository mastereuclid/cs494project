#include "irc_server.hpp"
// #include "responses.hpp"
#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <unordered_map>
using namespace irc::server;
using guard = std::lock_guard<std::mutex>;
using nickptr = std::shared_ptr<nick>;
using msgptr = std::unique_ptr<irc_msg>;
using chanptr = std::unique_ptr<channel>;
using std::string;
void no_command_found(nickptr user, msgptr msg);
//////////////////////////set dispatch hooks///////////////////////////
const std::unordered_map<std::string, std::function<void(nickptr, msgptr &&)>>
dispatch_table();
//////////////// data structures  //////////////////
// nicks
std::mutex nickex;
static std::unordered_map<std::string, nickptr> nicks;
// channels
std::mutex chanex;
static std::map<std::string, chanptr> channels;
std::atomic<uint> num_in_limbo = 0;
// command to function dispatch
static const std::unordered_map<std::string,
                                std::function<void(nickptr, msgptr &&)>>
    dispatch = dispatch_table();
static std::string servername = "server.name";
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

bool chan_exist(string chan) {
  guard lock(chanex);
  if (channels.count(chan) > 0)
    return true;
  return false;
}
const channel &get_channel(string chan) {
  guard lock(chanex);
  return *channels.at(chan).get();
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
    bool success = false;
    for (auto time_limit =
             std::chrono::system_clock::now() + std::chrono::seconds(15);
         std::chrono::system_clock::now() < time_limit;) {
      // check for incoming
      if (connection->msg_queue_empty()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        continue;
      } else {
        while (!connection->msg_queue_empty() && !success) {
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
            success = true;
            // we successfully inserted a nick and need to set the data in the
            // nick object
            connection->setnickname(nickname);
            connection->setusername(username);
            connection->setrealname(realname);
            // I think an motd function should probably go here
            motd(connection);
            break;
          }
        }
      }
      if (success)
        break;
    }
  } catch (const std::exception &e) {
    std::cout << "limbo: " << e.what() << std::endl;
  }
  num_in_limbo--;
}

///////////////////class definitionsm threading/////////////////////////////////
server::server(std::string name) { servername = name; }
void server::start_accepting_clients() {
  // accept_thread = std::thread(&server::engine, this);
  running = true;
  std::thread(&server::engine, this).detach();
  msg_thread = std::thread(&server::msg_engine, this);
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
  if (msg_thread.joinable())
    msg_thread.join();
}

uint server::conn_count() const { return nick_count(); }
uint server::limbo_count() const { return num_in_limbo; }

void server::engine() {
  // this is in its own thread so it needs to catch everything
  // std::cout << "omg what is happening\n";
  server_up.notify_all();
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
void server::msg_engine() {
  // std::unique_lock<std::mutex> lock(nickex, std::defer_lock);
  std::cout << "msg engine running" << std::endl;
  while (running) {
    // lock.lock();
    for (auto [nickname, nickdata] : nicks) {
      if (nickdata->msg_queue_empty())
        continue;
      // get the msg
      std::cout << "msg engine found data" << std::endl;
      msgptr msg = nickdata->get_next_irc_msg_ptr();
      // is there a dispatch func for the command? if not report an error
      if (dispatch.count(msg->command()) == 0) {
        no_command_found(nickdata, std::move(msg));
      } else {
        auto commandfunc = dispatch.at(msg->command());
        commandfunc(nickdata, std::move(msg));
      }
    }
    // lock.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
  std::cout << "msg engine stopped" << std::endl;
}
nick::nick(::socket &&sock) : protocol(std::move(sock)) {}
void nick::setnickname(std::string newnick) { nickname = std::move(newnick); }
void nick::setrealname(std::string newreal) { user = std::move(newreal); }
void nick::setusername(std::string newuser) { real = std::move(newuser); }
const std::string &nick::getnickname() const { return nickname; }
const std::string &nick::getrealname() const { return real; }
const std::string &nick::getusername() const { return user; }
channel::channel(std::string name) : chan_name(std::move(name)) {}
void channel::set_topic(std::string newtopic) { topic = std::move(newtopic); }
const string &channel::get_topic() const { return topic; }
void channel::join(nickptr user) {
  // check if banned or invite only, but I don't think I'm going to do those for
  // now
  bool oper = false;
  if (list_of_nicks.size() == 0)
    oper = true;
  list_of_nicks.insert(user);
  // broadcast the join
  std::stringstream msg;
  msg << ":" << user->getnickname() << " JOIN " << chan_name;
  broadcast(msg.str());
}
void channel::privmsg(nickptr user, const string &msg) const {
  // check if allowed to privmsg

  // build msg
  std::stringstream out;
  out << ":" << user->getnickname() << " PRIVMSG " << chan_name << " :" << msg;
  // broadcast
  broadcast(out.str());
}
void channel::broadcast(std::string msg) const {
  for (nickptr user : list_of_nicks) {
    user->sendircmsg(msg);
  }
}
////////////////////////command to function dispatch///////////////////////////

void join_channel(nickptr user, msgptr msg) {
  // is there a channel specified?
  guard lock(chanex);
  if (msg->middleparam().size() < 1) {
    user->sendircmsg(err_NEEDMOREPARAMS(msg->command()));
    return;
  }
  const string &chan_name = msg->middleparam().at(0);
  // is there a channel
  if (channels.count(chan_name) > 0) {
    // yes? check if allowed to enter
    channel &chan = *channels.at(chan_name).get();
    chan.join(user);
  } else {
    // no? create it and enter
    // channels.emplace(msg->middleparam().at(0)), chan_name);
    channels.insert({chan_name, std::make_unique<channel>(chan_name)});
    channel &chan = *channels.at(chan_name).get();
    chan.join(user);
  }
}
void privmsg(nickptr user, msgptr msg) {
  // no destination?
  if (msg->middleparam().size() == 0) {
    // if not report the error
    user->sendircmsg(err_NEEDMOREPARAMS(msg->command()));
    return;
  }
  // no data?
  if (msg->data().empty()) {
    user->sendircmsg(err_NOTEXTTOSEND());
  }
  // channel or nick?
  if (*msg->middleparam().at(0).begin() == '#') {
    // msg a channel
    guard lock(chanex);
    const channel &chan = *channels.at(msg->middleparam().at(0)).get();
    chan.privmsg(user, msg->data());
  } else {
    guard lock(nickex);
    // msg a user
    const nick &user_to = *nicks.at(msg->middleparam().at(0)).get();
    user_to.privmsg(user->getnickname(), msg->data());
  }
}
void no_command_found(nickptr user, msgptr msg) {
  user->sendircmsg(err_UNKNOWNCOMMAND(msg->command()));
}

void motd(nickptr user) {
  std::stringstream welmsg;
  welmsg << "001 " << user->getnickname() << " :Welcome to " << servername
         << " " << user->getnickname();
  user->sendircmsg(welmsg.str());
  welmsg.clear();
  welmsg << ":" << servername << " 375";
  user->sendircmsg("375");
}

///////////////////////dispatch table////////////////////////////////////////
const std::unordered_map<std::string, std::function<void(nickptr, msgptr &&)>>
dispatch_table() {
  std::unordered_map<std::string, std::function<void(nickptr, msgptr &&)>>
      dispatch;
  dispatch.emplace("PRIVMSG", privmsg);

  dispatch.emplace("JOIN", join_channel);
  // dispatch.emplace("no_command_found", no_command_found);
  return dispatch;
}