#include <exception>
#include <string>
#ifndef exceptions_h
#define exceptions_h

class addrinfo_fail : public std::exception {
  std::string cause;

public:
  addrinfo_fail(std::string arg) : cause(std::move(arg)) {}
  const char *what() const noexcept override { return cause.c_str(); }
};
class connection_failed : public std::exception {
public:
  const char *what() const noexcept override { return "couldn't connect\n"; }
};

class accept_fail : public std::exception {
  const char *what() const noexcept override {
    return "I should figure out errno...\n";
  }
};
#endif /* end of include guard:  */
