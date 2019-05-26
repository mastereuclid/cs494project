#include <string>
using std::string; // I know this isn't kosher in a header file. Deal with it.

string err_NOSUCHNICK(string arg);
string err_NOSUCHCHANNEL(string arg);
string err_UNKNOWNCOMMAND(string arg);
string err_NICKNAMEINUSE(string arg);
string err_NEEDMOREPARAMS(string arg);

string err_NOTEXTTOSEND();
string what();
