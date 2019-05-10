#include "client_socket.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

client_socket::client_socket(std::string hostname, int port) :
    sock(socket(AF_INET, SOCK_STREAM, 0)), port(port) {
    if (sock < 0) throw sock_failed();
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(port);
}

const char * bad_hostname::what() const throw() {
    return "couldn't resolve hostname\n";
}
const char * sock_failed::what() const throw() {
    return "sock failed to open\n";
}