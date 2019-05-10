#include <string>
#include <exception>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

class bad_hostname : public std::exception {
    const char * what()const throw() override;
};
class sock_failed : public std::exception {
    const char * what() const throw() override;
};

class client_socket {
    private:
    const int sock, port;
    sockaddr_in serv_addr;
    public:
    client_socket(std::string hostname, int port);
};