#ifndef proto_h
#define proto_h

#include "irc_msg.hpp"
#include "queue.hpp"
#include "socket.hpp"
#include <condition_variable>
#include <mutex>
// #include <queue>
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
  std::string parse_next_packet();
  std::string parse_next_packet(size_t pos);
  const std::string &get_storage();
  void insert_msg_into_queue();
  std::unique_ptr<irc_msg> get_next_irc_msg_ptr();
  bool msg_queue_empty() const;
  void launch_receiving_thread();
  void close();
  void sendircmsg(const std::string &msg);

private:
  void receive_engine();
  bool disconnected = false;
  std::string storage;
  std::thread iothread;
  std::mutex quetex;
  queue<irc_msg> fifo;
};
} // namespace irc

#endif /* end of include guard: proto_h */
