#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <client_socket.hpp>
#include <server_socket.hpp>
#include <string>
#include <thread>
// this was a nice starter test but I'm going to start throwing
// an exception on connection refusal and I can't test a server and client at
// the same time yet
// TEST_CASE("client socket exists", "[client]") {
//   client_socket sock("localhost", "9911");
// }

TEST_CASE("socket throws on bad hostname", "[client]") {
  REQUIRE_THROWS(client_socket("somebadhostname", "9911"));
}

TEST_CASE("connection refused", "[client]") {
  // this test assumes there is no process listening on port 999
  REQUIRE_THROWS(client_socket("localhost", "999"));
}
TEST_CASE("connect to google.com", "[client]") {

  REQUIRE_NOTHROW(client_socket("google.com", "80"));
}

TEST_CASE("send a message to myself", "[client][server]") {
  // threads... yay
  std::string msg("message to be sent");
  std::string recvd;
  std::string host;

  std::thread server([&msg, &host]() {
    server_socket server("3030");
    server.listen();
    connection client = server.accept();
    auto client2 = std::move(client);
    client2.send(msg);
    host = client2.hostname();
  });
  std::thread client([&recvd]() {
    client_socket client("localhost", "3030");
    recvd = client.receive();
  });
  server.join();
  client.join();
  REQUIRE(msg == recvd);
  REQUIRE(host == std::string("localhost"));
}
