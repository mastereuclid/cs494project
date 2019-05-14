#include "server_socket.hpp"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
server_socket::~server_socket() { close(sock); }
void server_socket::listen() {}
