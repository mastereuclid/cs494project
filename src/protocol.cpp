#include "protocol.hpp"
using namespace irc;
#include <iostream>
void protocol::packet(std::string &&packet) {
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
    fifo.push(std::make_unique<irc_msg>(parse_next_packet(term)));
    for (auto term = storage.find("\r\n"); term != std::string::npos;
         term = storage.find("\r\n")) {
      fifo.push(std::make_unique<irc_msg>(parse_next_packet(term)));
    }
  }
}
void protocol::sendircmsg(const std::string &msg) const {
  socket::send(msg + "\r\n");
}
std::unique_ptr<irc_msg> protocol::get_next_irc_msg_ptr() {
  if (fifo.empty())
    return std::unique_ptr<irc_msg>(nullptr);
  return fifo.pop();
}

void protocol::insert_msg_into_queue() {
  // fifo.emplace(irc_msg(parse_next_packet()));
  fifo.push(std::make_unique<irc_msg>(parse_next_packet()));
}

std::string protocol::parse_next_packet() {
  auto pos = storage.find("\r\n");
  return parse_next_packet(pos);
}
std::string protocol::parse_next_packet(size_t pos) {
  std::string complete_msg = storage.substr(0, pos);
  storage.erase(0, pos + 2);
  return complete_msg;
}
protocol::protocol() {}

const std::string &protocol::get_storage() { return storage; }

bool protocol::msg_queue_empty() const {
  bool test = fifo.empty();
  return test;
}
void protocol::launch_receiving_thread() {
  iothread = std::thread(&protocol::receive_engine, this);
}
void protocol::close() {
  disconnected = true;
  socket::close();
}

protocol::protocol(socket &&sock) : socket(std::move(sock)) {
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
    } catch (const connection_closed &e) {
      disconnected = true;

    } catch (const std::exception &e) {
      std::cout << e.what() << std::endl;
      fails++;
    }
  }
}

protocol::~protocol() {
  if (iothread.joinable())
    iothread.join();
}