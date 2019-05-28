#include "responses.hpp"
#include <sstream>
// using namespace irc::response;
const string err_NOSUCHNICK(const string arg) {
  return string("401 ") + arg + string(" :No such nick/channel");
}
const string err_NOSUCHCHANNEL(const string arg) {

  return string("403 ") + arg + string(" :No such nick/channel");
}
const string err_UNKNOWNCOMMAND(const string arg) {
  return string("421 ") + arg + string(" :Unknown command");
}

const string err_NICKNAMEINUSE(const string arg) {
  return string("433 * ") + arg + string(" :Nickname is already in use");
}
const string err_NEEDMOREPARAMS(const string arg) {
  return string("461 ") + arg + string(" :Not enough parameters");
}

const string err_NOTEXTTOSEND() { return string("412  :No text to send"); }

// string rpl_WELCOME(string arg)
const string rpl_quit(const string nickname, const string message) {
  std::stringstream out;
  out << ":" << nickname << " QUIT :" << message;
  return out.str();
}
const string rpl_privmsg(const string nickname, const string dest,
                         const string msg) {
  std::stringstream out;
  out << ":" << nickname << " " << dest << " :" << msg;
  return out.str();
}
