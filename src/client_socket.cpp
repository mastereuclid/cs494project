#include "client_socket.hpp"
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

client_socket::client_socket(std::string hostname, std::string port)
    : address(nullptr, freeaddrinfo) {
  addrinfo hints;
  addrinfo *temp = nullptr;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC; /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0; /* Any protocol */
  // resolve the domain name with getaddrinfo
  int error_code = getaddrinfo(hostname.c_str(), port.c_str(), &hints, &temp);
  address.reset(temp);
  if (error_code != 0)
    throw addrinfo_fail(gai_strerror(error_code));

  // try to connect, getaddriinfo can return multiple addresses, so we need to
  // loop through them
  addrinfo *try_this_address = nullptr;
  for (try_this_address = address.get(); try_this_address != nullptr;
       try_this_address = try_this_address->ai_next) {
    sock = socket(try_this_address->ai_family, try_this_address->ai_socktype,
                  try_this_address->ai_protocol);
    if (sock == -1)
      continue; // try the next address
    if (connect(sock, try_this_address->ai_addr,
                try_this_address->ai_addrlen) == -1) {
      close(sock);
      continue; // try the next address
    }
    // at this point we have created a sock and connected to it so we can exit
    // the loop
    break;
  }
  // but we might have exited the loop by running out of address to try to
  // connect to so we need to check
  if (try_this_address == nullptr)
    throw connection_failed();
}

client_socket::~client_socket() { close(sock); }

const char *bad_hostname::what() const noexcept {
  return "couldn't resolve hostname\n";
}
const char *sock_failed::what() const noexcept {
  return "sock failed to open\n";
}
