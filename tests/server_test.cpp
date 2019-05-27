// #define CATCH_CONFIG_MAIN
#include "irc_server.hpp"
#include <catch2/catch.hpp>
#include <chrono>
#include <iostream>
#include <thread>
TEST_CASE("class exists") {
  irc::server::server a;
  // REQUIRE(&a != nullptr);
}
/*
TEST_CASE("accept a connection", "[irc_server]") {
  irc::server::server server;
  server.start_accepting_clients();
  // std::this_thread::sleep_for(std::chrono::seconds(2));
  socket client, client2;

  client.connect("localhost", "6667");
  // client2.connect("localhost", "6667");
  std::this_thread::sleep_for(std::chrono::seconds(2));
  uint expect_one = server.limbo_count();
  //  while (server.conn_count() != 1)
  // std::this_thread::yield();
  server.stop_accepting_clients();
  // REQUIRE(test == std::string("a string"));
  // this was a temp test to make sure irc server could send a msg
  REQUIRE(expect_one == 1);
}*/

TEST_CASE("accept a nick", "[irc_server]") {
  std::cout << "sleeping for a couple seconds to free the socket\n";
  std::this_thread::sleep_for(std::chrono::seconds(2));
  irc::server::server server;
  server.start_accepting_clients();
  socket client;
  for (int attempts = 0; attempts < 3; attempts++) {
    try {
      client.connect("localhost", "6667");
      break;
    } catch (const connection_refused &e) {
    }
  }
  // std::this_thread::sleep_for(std::chrono::seconds(10));
  client.send("NICK albert\r\n");
  client.send("USER username hostname servername :al B\r\n");
  client.send("HELLO #what :hello there\r\n");
  // std::this_thread::sleep_for(std::chrono::seconds(3));
  string msg = client.receive();
  std::this_thread::sleep_for(std::chrono::seconds(200));

  std::cout << msg << std::endl;
  uint num_of_nicks = server.conn_count();
  server.stop_accepting_clients();
  client.close();
  REQUIRE(num_of_nicks == 1);
}
