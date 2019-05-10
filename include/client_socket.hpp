#include <string>
#include <exception>

class bad_hostname : public std::exception {
    const char * what()const throw() override;
};
class sock_failed : public std::exception {
    const char * what() const throw() override;
};

class client_socket {
    private:
    const int sock;
    public:
    client_socket(std::string hostname, int port);
};