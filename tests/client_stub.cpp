#include "socket.hpp"
#include <thread>
int main() {
  socket client;
  client.connect("localhost", "6667");
  client.send("NICK testclient\r\n");
  client.send("USER a b c :real\r\n");
  std::this_thread::sleep_for(std::chrono::seconds(1));
  client.send("JOIN #hi\r\n");
  client.send("PRIVMSG #hi :Hello there.\r\n");
  std::this_thread::sleep_for(std::chrono::seconds(2));
}