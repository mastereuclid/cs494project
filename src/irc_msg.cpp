#include "irc_msg.hpp"
#include <memory>
using std::string;
/*
<message> ::= [’:’ <prefix> <SPACE> ] <command> <params> <crlf>
<prefix> ::= <servername> | <nick> [ ’!’ <user> ] [ ’@’ <host> ]
<command> ::= <letter> { <letter> } | <number> <number> <number>
<SPACE> ::= ’ ’ { ’ ’ }
<params> ::= <SPACE> [ ’:’ <trailing> | <middle> <params> ]
<middle> ::= <Any *non-empty* sequence of octets not including SPACE
 or NUL or CR or LF, the first of which may not be ’:’>
<trailing> ::= <Any, possibly *empty*, sequence of octets not including
 NUL or CR or LF>
<crlf> ::= CR LF
*/
irc_msg::irc_msg() : my_type(type::CONTRUCTED) {}

irc_msg::irc_msg(std::string &&raw)
    : line(std::move(raw)), my_type(type::PARSED) {
  // check for prefix
  auto iter = line.begin();
  if (*iter == ':') {
    // we have a prefix
    std::string prefix = line.substr(1, line.find_first_of(" ") - 1);
    if (auto pos = prefix.find('!'); pos == std::string::npos) {
      my_prefix = prefix_type::SERVERNAME;
      // didn't find a ! so we have a servername
      from_servername = std::move(prefix);
    } else {
      my_prefix = prefix_type::USER;
      // we have nick!user@host
      auto point = prefix.find_first_of('!');
      auto at = prefix.find_first_of('@');
      prefix_nick = prefix.substr(0, point);
      prefix_user = prefix.substr(point + 1, (at) - (point + 1));
      // the second param for substr is a LENGTH not end pos. so (at) -
      // (point+1) is finish - start = distance
      prefix_host = prefix.substr(at + 1);
    }
  } else {
    my_prefix = prefix_type::NONE;
  }
}

const std::string &irc_msg::from_nick() { return prefix_nick; }
const string &irc_msg::from_user() { return prefix_user; }
const string &irc_msg::from_host() { return prefix_host; }
const irc_msg::prefix_type &irc_msg::from_type() { return my_prefix; }
