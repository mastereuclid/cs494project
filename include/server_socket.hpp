
#include <exceptions.hpp>
#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
class connection;
class server_socket {
private:
  int sock = 0;
  server_socket(int sock);

public:
  server_socket(std::string port);
  ~server_socket();
  // I need to defualt the move constructors
  void send(std::string /*, needs a destination, maybe not */);
  std::string recieve();
  void listen() const;
  connection accept() const;
};

class connection {
public:
  ~connection();
  // need to default the move constructors
  connection(connection &&) = default;
  void send(std::string) const;
  std::string receive() const;

private:
  int sock = 0;
  std::unique_ptr<sockaddr> address;
  connection(int sock, std::unique_ptr<sockaddr> peer_address);
  friend connection server_socket::accept() const;
};
