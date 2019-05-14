
#include <exceptions.hpp>
#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
class server_socket {
private:
  int sock = 0;

public:
  server_socket(std::string port);
  ~server_socket();
  // I need to defual the move constructors
  void send(std::string /*, needs a destination */);
  std::string recieve();
  void listen();
};
