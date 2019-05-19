
#include <exceptions.hpp>
#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

class connection;
bool operator<(const connection &lhs, const connection &rhs);
class server_socket {
private:
  int sock = 0;
  server_socket(int sock);

public:
  server_socket(std::string port);
  ~server_socket();
  server_socket(server_socket &&) = default;
  server_socket(const server_socket &) = delete;
  server_socket &operator=(const server_socket &) = delete;
  void send(std::string /*, needs a destination, maybe not */);
  std::string recieve();
  void listen() const;
  connection accept() const;
};
// I might need to make this sortable
class connection {

public:
  ~connection();
  // need to default the move constructors
  connection(connection &&);
  connection() = delete;
  void send(std::string) const;
  std::string receive() const;
  const std::string &hostname() const;
  void close();

private:
  bool moved = false;
  bool running = true;
  int sock = 0;
  mutable bool hostcached = false;
  mutable std::string host;
  std::unique_ptr<sockaddr> address;
  connection(int sock, std::unique_ptr<sockaddr> peer_address);
  friend connection server_socket::accept() const;
  friend bool operator<(const connection &lhs, const connection &rhs);
};
