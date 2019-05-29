#pragma once
#include "irc_msg.hpp"
struct message {
  std::string from;
  irc_msg msg;
  message(std::string nick, irc_msg msg_arg)
      : from(std::move(nick)), msg(std::move(msg_arg)) {}
};