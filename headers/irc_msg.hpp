#include <string>
#include <vector>
using mparams_t = std::vector<std::string>;
class irc_msg {
public:
  enum class prefix_type { NONE, SERVERNAME, USER };
  irc_msg(std::string &&line);
  irc_msg();
  const std::string &from_nick() const;
  const std::string &from_user() const;
  const std::string &from_host() const;
  const prefix_type &from_type() const;
  const std::string &command() const;
  const std::string &params() const;
  const std::string &data() const;
  const mparams_t &middleparam() const;

private:
  std::string line;
  prefix_type my_prefix;
  // prefix: NONE or SERVERNAME or nick!user@host
  std::string from_servername;
  std::string prefix_nick;
  std::string prefix_user;
  std::string prefix_host;
  mparams_t mparams;
  uint num_params = 0;
  //<SPACE> ::= ’ ’ { ’ ’ }
  // need a function to eat through whitespace

  //<command> ::= <letter> { <letter> } | <number> <number> <number>
  // enum class command_type { WORD, DIGITS };
  std::string cmd;

  //<params> ::= <SPACE> [ ’:’ <trailing> | <middle> <params> ]
  // enum class params_type { MIDDLE, TRAILING };
  // irc params are recurisve...
  std::string middle;
  std::string trailing;
  void split(std::string);
};
