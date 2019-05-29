#ifndef proto_h
#define proto_h

#include "irc_msg.hpp"
// #include "queue.hpp"
#include "socket.hpp"
#include <condition_variable>
#include <mutex>
// #include <queue>
#include <deque>
#include <string>
#include <thread>
namespace irc {

class protocol : public socket {
public:
  protocol();
  protocol(std::string hostname, std::string port = "6667");
  protocol(socket &&sock);
  protocol(protocol &&) = delete;
  protocol(const protocol &) = delete;
  ~protocol();
  void packet(std::string &&line);

  std::string parse_next_packet(size_t pos);

  irc_msg get_next_irc_msg();
  bool msg_queue_empty() const;
  void launch_receiving_thread();
  void close();
  void sendircmsg(const std::string &msg) const;

private:
  const std::string &get_storage();
  std::deque<irc_msg> queue;
  void receive_engine();
  bool disconnected = false;
  std::string storage;
  std::thread iothread;
  mutable std::mutex quetex;
  // queue<irc_msg> fifo;
};
} // namespace irc

#endif /* end of include guard: proto_h */
