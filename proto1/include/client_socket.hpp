
#include <exceptions.hpp>
#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
class client_socket {
private:
  std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> address;
  int sock = 0;
  bool moved = false;
  bool closed = false;

public:
  client_socket(std::string hostname, std::string port);
  void send(std::string) const;
  std::string receive() const;
  client_socket(const client_socket &) = delete;
  client_socket &operator=(const client_socket &) = delete;
  void close();
  client_socket(client_socket &&other);
  // need to default the move operators
  ~client_socket();
};