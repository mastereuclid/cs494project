#include <cstring>
#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
class addressdeleter {
public:
  void operator()(addrinfo *ad) { freeaddrinfo(ad); }
};
class bla {
  struct hi {
    int a;
    int b;
  };
  hi hello = {hello.b = 2, hello.a = 1};
  // hello.b = 3;
  // hello.a = 1;
  std::unique_ptr<addrinfo, addressdeleter> address;
  std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> result;

public:
  bla() : address(nullptr, addressdeleter()), result(nullptr, freeaddrinfo) {
    // std::unique_ptr<addrinfo, addressdeleter> result(nullptr,
    // addressdeleter());
    addrinfo hints;
    addrinfo *temp;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    getaddrinfo("google.com", "80", &hints, &temp);
    result.reset(temp);
  }
};

void testserver() {
  addrinfo hints, *temp;
  int sock = 0, clientsock = 0;
  std::string port = std::string("3030");
  std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> address(nullptr,
                                                             freeaddrinfo);
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET; // ipv4 only
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0; /* Any protocol */
  int error_code = getaddrinfo(nullptr, port.c_str(), &hints, &temp);
  address.reset(temp);
  if (error_code != 0)
    throw gai_strerror(error_code);
  for (addrinfo *res = address.get(); res != nullptr; res = res->ai_next) {
    char hostname[1025];
    int error = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, NI_MAXHOST,
                            NULL, 0, 0);
    if (error != 0) {
      fprintf(stderr, "error in getnameinfo: %s\n", gai_strerror(error));
      continue;
    }
    if (*hostname != '\0')
      printf("hostname: %s\n", hostname);
  }
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
  listen(sock, 1);
  sockaddr clientaddr;
  memset(&clientaddr, 0, sizeof(struct addrinfo));
  socklen_t client_address_size = sizeof(clientaddr);
  clientsock = accept(sock, &clientaddr, &client_address_size);
  char buffer[1024];
  int length = recv(clientsock, &buffer, 1024, 0);
  if (length != -1)
    puts(buffer);
}

int main() {
  testserver();
  return 0;
}
