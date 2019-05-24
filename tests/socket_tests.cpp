

// #define CATCH_CONFIG_MAIN
#include "socket.hpp"
#include <catch2/catch.hpp>
#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
using std::string;
using std::thread;
TEST_CASE("connect to google", "[socket][client]") {
  socket a;
  a.connect("google.com", "80");
  a.close();
  // REQUIRE(a.socketfd() > 0);
}
TEST_CASE("fail to connect to a bad hostname", "[socket][client]") {
  socket a;
  REQUIRE_THROWS(a.connect("badhostname", "1000"));
  // REQUIRE_THROWS_AS(a.connect("badhostname", "1000"),
  //                   could_not_resolve_address);
}
/*
this test was used with comp_test and succeed
TEST_CASE("open a server socket and wait") {
  auto msg = string("test messge from server to client");
  socket server;
  server.bind("3030");
  socket client = server.accept();
  client.send(msg);
  std::string recvd = client.receive();
  REQUIRE(recvd == std::string("test from client to server"));
}
*/
/* bad test
TEST_CASE("open and bind a socket", "[socket][server]") {
  std::mutex mlock;
  auto msg = string("test messge from server to client");
  string recv;
  auto server = [&msg, &mlock]() {
    // std::unique_lock<std::mutex> safety(mlock);
    socket server;
    server.bind("3030");
    // safety.unlock();
    socket client = server.accept();
    std::cout << client.hostname() << '\n';
    client.send(msg);
    return 5;
  };
  auto t1 = std::async(std::launch::async, server);
  // auto t2 = std::async(std::launch::async, client);
  // std::this_thread::sleep_for(std::chrono::seconds(2));
  std::this_thread::yield();
  // std::unique_lock<std::mutex> safety(mlock);
  // safety.lock();
  socket clientsock;
  for (int loop = 0; loop < 3; loop++) {
    try {
      clientsock.connect("localhost", "3030");
      break;
    } catch (const connection_refused &e) {
      std::this_thread::yield();
    }
  }
  recv = clientsock.receive();
  t1.get();
  // t2.get();
  REQUIRE(recv == msg);
}
*/

// TEST_CASE("open and bind a socket", "[socket][server]") {
//   auto msg = string("test messge from server to client");
//   string recv;
//   std::mutex mlock;
//   auto server = [&msg, &mlock]() {
//     try {
//       std::unique_lock<std::mutex> safety(mlock);
//       socket server;
//       server.bind("3030");
//       safety.unlock();
//       socket client = server.accept();
//       client.send(msg);
//     } catch (const std::exception &e) {
//       std::cout << e.what();
//     }
//     return 5;
//   };
//   auto client = [&recv, &mlock]() {
//     // std::this_thread::yield();
//     try {
//       std::unique_lock<std::mutex> safety(mlock);
//       std::this_thread::sleep_for(std::chrono::seconds(2));
//       socket client;
//       client.connect("localhost", "3030");
//       recv = client.receive();
//     } catch (const std::exception &e) {
//       std::cout << e.what();
//     }
//     return 5;
//   };
//   // auto t1 = std::async(std::launch::async, server);
//   auto t1 = thread(client);
//   int a = server();
//   // auto t2 = std::async(client);
//   // try {
//   // t2.get();
//   t1.join();
//   REQUIRE(msg == recv);
//   // }
//   // std::async(std::launch::async, _Fn && __fn, _Args && __args...)
//   //     thread t1([]() {
//   //       socket server;
//   //       server.bind("3030");
//   //       server.accept();
//   //     });
//   // thread t2([]() {
//   //   socket client;
//   //   client.connect("localhost", "3030");
//   // });
//   // t1.join();
//   // t2.join();
// }
