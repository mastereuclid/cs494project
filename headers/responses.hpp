#include <string>
using std::string; // I know this isn't kosher in a header file. Deal with it.

// string rpl_WELCOME(string arg);
const string rpl_quit(const string nickname, const string message);
const string rpl_privmsg(const string nickname, const string dest,
                         const string msg);
const string err_NOSUCHNICK(const string arg);
const string err_NOSUCHCHANNEL(const string arg);
const string err_UNKNOWNCOMMAND(const string arg);
const string err_NICKNAMEINUSE(const string arg);
const string err_NEEDMOREPARAMS(const string arg);

const string err_NOTEXTTOSEND();
