#include "protocol.hpp"
using namespace irc;
#include <iostream>
void protocol::packet(std::string&& packet) {
  storage.append(packet);
  // looks for a delimiter
  auto term = storage.find("\r\n");
  // if not there save for later use
  if (term == std::string::npos) {
    // there is no complete message yet
    return;
  } else {
    // parse_next_packet(term);
    // auto bla = parse_next_packet(term);
    std::lock_guard<std::mutex> lock(quetex);
    queue.emplace_back(parse_next_packet(term));
    for (auto term = storage.find("\r\n"); term != std::string::npos; term = storage.find("\r\n")) {
      queue.emplace_back(parse_next_packet(term));
    }
  }
}
void protocol::sendircmsg(const std::string& msg) const {
  if (disconnected)
    return;
  socket::send(msg + "\r\n");
}
irc_msg protocol::get_next_irc_msg() {
  std::lock_guard<std::mutex> lock(quetex);
  irc_msg temp(queue.front());
  queue.pop_front();
  return temp;
}

std::string protocol::parse_next_packet(size_t pos) {
  std::string complete_msg = storage.substr(0, pos);
  storage.erase(0, pos + 2);
  return complete_msg;
}
protocol::protocol() {}

const std::string& protocol::get_storage() { return storage; }

bool protocol::msg_queue_empty() const {
  // std::lock_guard<std::mutex> lock(quetex);
  return queue.empty();
}
void protocol::launch_receiving_thread() {
  iothread = std::thread(&protocol::receive_engine, this);
}
void protocol::close() {
  disconnected = true;
  socket::close();
}

protocol::protocol(socket&& sock) : socket(std::move(sock)) { launch_receiving_thread(); }

protocol::protocol(std::string hostname, std::string port) {
  connect(hostname, port);
  launch_receiving_thread();
}

void protocol::receive_engine() {
  // own thread catch everything
  int fails = 0;
  while (!disconnected && fails < 3) {
    try {
      // std::string line = socket::receive();
      // std::cout << line << std::endl;
      // packet(std::move(line));
      packet(socket::receive());

    } catch (const receive_fail& e) {
      if (e.errnum() == 11) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        continue;
      }
      std::cout << "protocol engine" << e.errnum() << ": " << e.what() << std::endl;
      disconnected = true;
      close();
      if (e.errnum() == 104) {
        packet("QUIT :client closed connection\r\n");
      }
    }

    catch (const connection_closed& e) {
      disconnected = true;
      close();

    } catch (const std::exception& e) {
      std::cout << "protocol engine: " << e.what() << std::endl;
      fails++;
    }
  }
  disconnected = true;
  close();
}

protocol::~protocol() {
  if (iothread.joinable())
    iothread.join();
}