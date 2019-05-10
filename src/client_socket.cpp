#include "client_socket.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
client_socket::client_socket(std::string hostname, int port) :
    sock(socket(AF_INET, SOCK_STREAM, 0)) {
    if (sock < 0) throw sock_failed();
    
}

const char * bad_hostname::what() const throw() {
    return "couldn't resolve hostname\n";
}
const char * sock_failed::what() const throw() {
    return "sock failed to open\n";
}