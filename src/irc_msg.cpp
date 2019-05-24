#include "irc_msg.hpp"
#include <iterator>
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
// irc_msg::irc_msg() {}
irc_msg::irc_msg() {}
irc_msg::irc_msg(std::string &&raw) : line(std::move(raw)) {
  // check for prefix
  // auto iter = line.begin();
  string remainder = line;
  if (line.at(0) == ':') {
    // we have a prefix
    std::string prefix = line.substr(1, line.find_first_of(" ") - 1);
    remainder = line.substr(prefix.length() + 1);
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
  // ok prefix is done what next?
  // iter = line.find_first_of(' ');
  // I want to advance the iterator until we are no longer pointing at spaces.
  auto command_start = remainder.find_first_not_of(' ');
  auto command_end = remainder.find_first_of(' ', command_start);
  this->cmd = remainder.substr(command_start, command_end - command_start);
  remainder = remainder.substr(command_end + 1);
  remainder = remainder.substr(remainder.find_first_not_of(' '));
  ;
  // now the middle and trailing
  auto colon = remainder.find(':');
  if (colon == string::npos) {
    // no trailing data
    middle = remainder;
  } else {
    middle = remainder.substr(0, colon - 1);
    trailing = remainder.substr(colon + 1);
  }
  split(middle);
}
// a list of words
void irc_msg::split(string middle) {
  // split a string by spaces and place each part into a vector

  auto start = middle.find_first_not_of(' ');
  auto space = middle.find_first_of(' ', start);
  while (start != string::npos) {
    mparams.emplace_back(middle.substr(start, space - start));
    start = middle.find_first_not_of(' ', space);
    space = middle.find_first_of(' ', start);
  }

  // num_params = loop;
}

const mparams_t &irc_msg::middleparam() const { return mparams; }

const std::string &irc_msg::from_nick() const { return prefix_nick; }
const string &irc_msg::from_user() const { return prefix_user; }
const string &irc_msg::from_host() const { return prefix_host; }
const irc_msg::prefix_type &irc_msg::from_type() const { return my_prefix; }
const string &irc_msg::command() const { return cmd; }
const string &irc_msg::params() const { return middle; }
const string &irc_msg::data() const { return trailing; }
