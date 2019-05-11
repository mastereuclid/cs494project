#include "client_socket.hpp"
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

client_socket::client_socket(std::string hostname, std::string port) {
  addrinfo hints;
  addrinfo *result, *rp, *self;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
  hints.ai_flags = 0;
  hints.ai_protocol = 0; /* Any protocol */
  int error_code = getaddrinfo(hostname.c_str(), port.c_str(), &hints, &result);
  getaddrinfo("localhost", NULL, &hints, &self);
  if (error_code != 0)
    throw addrinfo_fail(gai_strerror(error_code));
  using std::cout;
  cout << "sa data\n"
       << self->ai_addr->sa_data << '\n'
       << result->ai_addr->sa_data << '\n';
  if (std::strcmp(self->ai_addr->sa_data, result->ai_addr->sa_data) &&
      hostname != std::string("localhost"))
    throw bad_hostname();
  freeaddrinfo(result);
}

client_socket::~client_socket() {}

const char *bad_hostname::what() const noexcept {
  return "couldn't resolve hostname\n";
}
const char *sock_failed::what() const noexcept {
  return "sock failed to open\n";
}