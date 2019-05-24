#include "socket.hpp"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
// #include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <netdb.h>
#include <sys/socket.h>
constexpr uint buffer_size = 1024; // A gloabal. Am I insane? Its const...
socket::socket() {}

void socket::connect(std::string host, std::string port) {
  if (sock != -1)
    throw socket_in_use();
  addrinfo *address = nullptr;
  std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> addressptr(nullptr,
                                                                freeaddrinfo);
  addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM; // TCP
  hints.ai_flags = 0;              // FLAGs are always 0
  hints.ai_protocol = 0; // lets the system choose the most appropraite proto
  int error_code = getaddrinfo(host.c_str(), port.c_str(), &hints, &address);
  addressptr.reset(address);
  if (error_code != 0)
    throw could_not_resolve_address();
  addrinfo *try_this_address = nullptr;
  for (try_this_address = address; try_this_address != nullptr;
       try_this_address = try_this_address->ai_next) {
    sock = ::socket(try_this_address->ai_family, try_this_address->ai_socktype,
                    try_this_address->ai_protocol);
    if (sock == -1)
      continue; // try the next address
    if (::connect(sock, try_this_address->ai_addr,
                  try_this_address->ai_addrlen) == -1) {
      ::close(sock);
      continue; // try the next address
    }
    // at this point we have created a sock and connected to it so we can exit
    // the loop
    break;
  }
  if (try_this_address == nullptr)
    throw connection_refused();
  open = true;
  ///////////////////////save resolve info///////////////////////////////
}
void socket::close() {
  if (sock != -1)
    ::close(sock);
  open = false;
  sock = -1;
}
socket::~socket() { close(); }
socket::socket(socket &&other) : sock(other.sock), open(other.open) {
  other.sock = -1;
}

void socket::bind(std::string port, std::string host) {
  if (sock != -1)
    throw socket_in_use();
  addrinfo hints, *address = nullptr;
  std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> addressptr(nullptr,
                                                                freeaddrinfo);
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET; /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0; /* Any protocol */
  int error_code = getaddrinfo(host.c_str(), port.c_str(), &hints, &address);
  addressptr.reset(address);
  if (error_code != 0)
    throw addrinfo_fail(gai_strerror(error_code));
  addrinfo *res = address;
  int bindrv = 0;
  for (; res != nullptr; res = res->ai_next) {
    sock = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
      continue;
      // this result failed try the next.
    }
    if (bindrv = ::bind(sock, res->ai_addr, res->ai_addrlen); bindrv == -1) {
      ::close(sock);
      continue;
      // failed to bind try next address
    }
    break; // we have opened a sock and bound
  }
  if (bindrv == -1) {
    throw bind_fail(strerror(errno));
  }

  int rv = listen(sock, 10);
  if (rv == -1) {
    std::cout << "sock: " << sock << '\n';
    throw listen_fail(strerror(errno));
  }
  open = true;
  ///////////////////////////////////////
  // need to save the resolve stuff
}

void socket::send(std::string message) const {
  if (sock == -1)
    throw socket_not_open();
  int rv = ::send(sock, message.c_str(), message.length(), 0);
  if (rv == -1) {
    // check errno
    throw send_fail(strerror(errno));
  } else if (ulong sent_length = rv; sent_length < message.length()) {
    // we have a problem. We need to send the remaining data...
  }
}

int socket::sockfd() const { return sock; }

std::string socket::receive() const {
  if (sock == -1)
    throw socket_not_open();
  char buffer[buffer_size];
  int rv = ::recv(sock, &buffer, buffer_size, 0);
  if (rv == -1)
    throw receive_fail(strerror(errno));
  else if (rv == 0) {
    throw connection_closed();
  }
  return std::string(buffer, rv);
}

class socket socket::accept() const {
  auto their_address = std::make_unique<sockaddr>();
  socklen_t size = sizeof(sockaddr);
  int con = ::accept(sock, their_address.get(), &size);
  if (con == -1)
    throw accept_fail(strerror(errno));

  char host[1024];
  char service[20];

  getnameinfo(their_address.get(), sizeof(sockaddr), host, sizeof host, service,
              sizeof service, 0);
  return socket(con, std::string(host));
}
const std::string &socket::hostname() const { return hostname_connected_to; }

socket::socket(int con, std::string &&host)
    : sock(con), hostname_connected_to(std::move(host)), open(true) {}
socket::socket(int con) : sock(con), open(true) {}
