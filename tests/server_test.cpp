// #define CATCH_CONFIG_MAIN
#include "irc_server.hpp"
#include <catch2/catch.hpp>
#include <chrono>
#include <thread>
TEST_CASE("class exists") {
  irc::server::server a;
  // REQUIRE(&a != nullptr);
}
/*
TEST_CASE("accept a connection", "[irc_server]") {
  irc::server::server server;
  server.start_accepting_clients();
  std::this_thread::sleep_for(std::chrono::seconds(2));
  socket client, client2;
  client.connect("localhost", "6667");
  // client2.connect("localhost", "6667");
  //  while (server.conn_count() != 1)
  std::this_thread::yield();
  uint client_count = server.conn_count();
  server.stop_accepting_clients();
  // REQUIRE(test == std::string("a string"));
  // this was a temp test to make sure irc server could send a msg
  REQUIRE(client_count == 1);
}
*/
// TEST_CASE("accept a nick", "[irc_server]") {
//   irc::server::server server;
//   server.start_accepting_clients();
//   socket client;
//   client.connect("localhost", "6667");
//   client.send("NICK albert\r\n");
//   std::this_thread::sleep_for(std::chrono::seconds(10));
//   client.send("USER username hostname servername :al B\r\n");
// }
