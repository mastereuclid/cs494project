#include <exception>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

class bad_hostname : public std::exception {
public:
  const char *what() const noexcept override;
};
class sock_failed : public std::exception {
public:
  const char *what() const noexcept override;
};
class addrinfo_fail : public std::exception {
  std::string cause;

public:
  addrinfo_fail(std::string arg) : cause(std::move(arg)) {}
  const char *what() const noexcept override { return cause.c_str(); }
};

class client_socket {
private:
  int sock;

public:
  client_socket(std::string hostname, std::string port);
  ~client_socket();
};