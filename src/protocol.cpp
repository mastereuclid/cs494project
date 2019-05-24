#include "protocol.hpp"
using namespace irc;

void protocol::packet(std::string packet) {
  storage.append(packet);
  // looks for a delimiter
  auto term = storage.find("\r\n");
  // if not there save for later use
  if (term == std::string::npos) {
    // there is no complete message yet
    return;
  } else {
    // parse_next_packet(term);
    auto bla = parse_next_packet(term);
    fifo.push(std::make_unique<irc_msg>(std::move(bla)));
  }
}

std::unique_ptr<irc_msg> protocol::get_next_msg() {
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