#ifndef socket_h
#define socket_h

#include <exception>
#include <string>
class socket {
public:
  socket();
  void connect(std::string host, std::string port);
  // int socketfd();
  void close();
  void bind(std::string, std::string = "localhost");
  void send(std::string) const;
  std::string receive() const;
  socket accept() const;
  ~socket();
  socket(socket &&);
  const std::string &hostname() const;
  // void bind(std)
protected:
  int sockfd() const;

private:
  socket(int con, std::string &&);
  socket(int con);
  int sock = -1;
  mutable std::string hostname_connected_to;
  std::string port;
  // std::string resolved_host;
  mutable bool open = false;
};

class socket_in_use : public std::exception {
public:
  const char *what() const noexcept override { return "socket already open"; }
};
class socket_not_open : public std::exception {
public:
  const char *what() const noexcept override {
    return "can't send or receive before opening the socket";
  }
};
class could_not_resolve_address : public std::exception {
public:
  const char *what() const noexcept override {
    return "could not resolve address";
  }
};
class connection_refused : public std::exception {
public:
  const char *what() const noexcept override { return "connection refused"; }
};
class addrinfo_fail : public std::exception {
  std::string error;

public:
  addrinfo_fail(const char *err) : error(err) {}
  const char *what() const noexcept override { return error.c_str(); }
};
class accept_fail : public std::exception {
public:
  accept_fail(std::string err) {
    error = std::string("accept failed: ").append(err);
  }
  const char *what() const noexcept override { return error.c_str(); }

private:
  std::string error;
};
class listen_fail : public std::exception {
public:
  listen_fail(std::string err) {
    error = std::string("listen failed: ").append(err);
  }
  const char *what() const noexcept override { return error.c_str(); }

private:
  std::string error;
};
class receive_fail : public std::exception {
public:
  receive_fail(std::string err) {
    error = std::string("receive failed: ").append(err);
  }
  const char *what() const noexcept override { return error.c_str(); }

private:
  std::string error;
};
class connection_closed : public std::exception {
public:
  const char *what() const noexcept override {
    return "connection has been closed";
  }
};
class bind_fail : public std::exception {
public:
  bind_fail(std::string err) {
    error = std::string("bind failed: ").append(err);
  }
  const char *what() const noexcept override { return error.c_str(); }

private:
  std::string error;
};
class send_fail : public std::exception {
public:
  send_fail(std::string err, int errnum) : num(errnum) {
    error = std::string("send failed: ").append(err);
  }
  const char *what() const noexcept override { return error.c_str(); }
  const int errnum() const { return num; }

private:
  int num = 0;
  std::string error;
};
#endif /* end of include guard:  */
