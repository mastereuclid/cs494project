#include "responses.hpp"
// using namespace irc::response;
string err_NOSUCHNICK(string arg) {
  return string("401 ") + arg + string(" :No such nick/channel");
}
string err_NOSUCHCHANNEL(string arg) {

  return string("403 ") + arg + string(" :No such nick/channel");
}
string err_UNKNOWNCOMMAND(string arg) {
  return string("421 ") + arg + string(" :Unknown command");
}
string what() { return string("wtf"); }

string err_NICKNAMEINUSE(string arg) {
  return string("433 ") + arg + string(" :Nickname is already in use");
}
string err_NEEDMOREPARAMS(string arg) {
  return string("461 ") + arg + string(" :Not enough parameters");
}

string err_NOTEXTTOSEND() { return string("412  :No text to send"); }

// string rpl_WELCOME(string arg)