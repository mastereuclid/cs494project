#include <cstring>
#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
// condition_variable::wait (with predicate)
#include <chrono>
#include <condition_variable> // std::condition_variable
#include <iostream>           // std::cout
#include <mutex>              // std::mutex, std::unique_lock
#include <thread>             // std::thread, std::this_thread::yield
std::mutex mtx;
std::condition_variable cv;

int cargo = 0;
bool shipment_available() { return cargo != 0; }

void consume(int n) {
  for (int i = 0; i < n; ++i) {
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck, shipment_available);
    // consume:
    std::cout << cargo << '\n';
    cargo = 0;
  }
}

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
  int sock = 0, clientsock = 0, clientsock2 = 0;
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
  sockaddr clientaddr, clientaddr2;
  memset(&clientaddr, 0, sizeof(struct addrinfo));
  memset(&clientaddr2, 0, sizeof(struct addrinfo));
  socklen_t client_address_size = sizeof(clientaddr);
  socklen_t client_address_size2 = sizeof(clientaddr2);
  clientsock = accept(sock, &clientaddr, &client_address_size);
  clientsock2 = accept(sock, &clientaddr2, &client_address_size2);
  // char buffer[1024];
  // int length = recv(clientsock, &buffer, 1024, 0);
  // if (length != -1)
  //   puts(buffer);
  const char *msg = "what happens when data is sent to the listening socket";
  int len = strlen(msg), bytesent = 0;
  bytesent = send(clientsock, msg, len, 0);
  close(sock);
  close(clientsock);
  close(clientsock2);
}

void sizetest() {
  // char bla[1024];
  printf("%lu", sizeof(char[1024]));
}
class dest_test {
public:
  dest_test() {}
  ~dest_test() { std::cout << "dest_test\n"; }
};

class engine {
public:
  engine() {
    // std::thread(&engine::run, this).detach();
    t = std::thread(&engine::run, this);
    // t.detach();
  }
  ~engine() { t.join(); }
  void run() {
    dest_test b;
    for (; a < 100; a++) {
      if (a % 5 == 0)
        std::cout << std::this_thread::get_id() << " " << a << '\n';
      std::this_thread::yield();
    }
  }

private:
  std::thread t;
  size_t a = 2;
};
void threadtest() {
  engine enginetest;
  // std::this_thread::yield();
  engine eng2;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  for (volatile size_t i = 0; i < 20; i++) {

    std::this_thread::yield();
    std::cout << " I can still output here right...\n";
  }
}

int main() { threadtest(); }
