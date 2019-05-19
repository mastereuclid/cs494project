#include "server_socket.hpp"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
constexpr uint buffer_size = 1024; // A gloabal. Am I insane? Its const...
server_socket::server_socket(std::string port) {
  addrinfo hints, *temp;
  std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> address(nullptr,
                                                             freeaddrinfo);
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET; /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0; /* Any protocol */
  int error_code = getaddrinfo(nullptr, port.c_str(), &hints, &temp);
  address.reset(temp);
  if (error_code != 0)
    throw addrinfo_fail(gai_strerror(error_code));

  for (addrinfo *res = address.get(); res != nullptr; res = res->ai_next) {
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
      continue;
      // this result failed try the next.
    }
    if (bind(sock, res->ai_addr, res->ai_addrlen) == -1) {
      close(sock);
      continue;
      // failed to bind try next address
    }
    break; // we have opened a sock and bound
  }
}
server_socket::server_socket(int sock) : sock(sock) {}
connection server_socket::accept() const {
  auto their_address = std::make_unique<sockaddr>();
  socklen_t size = sizeof(sockaddr);
  int con = ::accept(sock, their_address.get(), &size);
  if (con == -1)
    throw accept_fail();
  return connection(con, std::move(their_address));
}

server_socket::~server_socket() { close(sock); }
void server_socket::listen() const { ::listen(sock, 10); }
connection::connection(connection &&other)
    : sock(other.sock), hostcached(other.hostcached),
      host(std::move(other.host)), address(std::move(other.address)) {
  other.moved = true;
}
connection::~connection() {
  if (!moved && !running)
    ::close(sock);
}
connection::connection(int sock, std::unique_ptr<sockaddr> their_address)
    : sock(sock), address(std::move(their_address)) {}

void connection::send(std::string msg) const {
  int rv = ::send(sock, msg.c_str(), msg.length(), 0);
  if (rv == -1)
    throw send_fail();
  if (unsigned long bytesent = rv; bytesent < msg.length()) {
    // send the rest of the data...
  }
}
std::string connection::receive() const {
  char buffer[buffer_size];
  int rv = ::recv(sock, &buffer, buffer_size, 0);
  if (rv == -1)
    throw receive_fail();
  else if (rv == 0) {
    throw connection_closed();
  }
  return std::string(buffer, rv);
}
const std::string &connection::hostname() const {
  // int getpeername(int sockfd, struct sockaddr *addr, int *addrlen);
  if (hostcached)
    return host;
  auto addr = std::make_unique<sockaddr>();
  // socklen_t addrsize = sizeof(sockaddr);
  auto service = std::make_unique<char[]>(1024);
  auto gethost = std::make_unique<char[]>(1024);
  // int rv = getpeername(sock, addr.get(), &addrsize);
  // if (rv == -1)
  // throw "getpeername failed";
  int rv = getnameinfo(address.get(), sizeof(sockaddr), gethost.get(), 1024,
                       service.get(), 1024, 0);
  if (rv == -1)
    throw "getnameinfo failed";
  /* I thought about getting the client's IP address but its not worth it
char ipstr[INET6_ADDRSTRLEN];
if (addr.get()->sa_family == AF_INET) {
  inet_ntop(AF_INET, addr.get()->sin_add, ipstr, sizeof ipstr);
} else { // AF_INET6
  inet_ntop(AF_INET6, addr.get()->sin6_addr, ipstr, sizeof ipstr);
}
*/
  host = std::string(gethost.get());
  hostcached = true;
  return host;
}
void connection::close() {
  ::close(sock);
  running = false;
}

bool operator<(const connection &lhs, const connection &rhs) {
  return lhs.sock < rhs.sock;
}
