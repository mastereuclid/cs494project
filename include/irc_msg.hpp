#include <string>

class irc_msg {
public:
  enum class prefix_type { NONE, SERVERNAME, USER };
  irc_msg(std::string &&line);
  irc_msg();
  const std::string &from_nick();
  const std::string &from_user();
  const std::string &from_host();
  const prefix_type &from_type();

private:
  std::string line;
  prefix_type my_prefix;
  // prefix: NONE or SERVERNAME or nick!user@host
  enum class type { PARSED, CONTRUCTED } const my_type;
  std::string from_servername;
  std::string prefix_nick;
  std::string prefix_user;
  std::string prefix_host;
  //<SPACE> ::= ’ ’ { ’ ’ }
  // need a function to eat through whitespace

  //<command> ::= <letter> { <letter> } | <number> <number> <number>
  enum class command_type { WORD, DIGITS };
  std::string command;

  //<params> ::= <SPACE> [ ’:’ <trailing> | <middle> <params> ]
  // enum class params_type { MIDDLE, TRAILING };
  // irc params are recurisve...
  std::string middle;
  std::string trailing;
};
