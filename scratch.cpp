#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
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

int main() {
  bla hi;
  return 0;
}
