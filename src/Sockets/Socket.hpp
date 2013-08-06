
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>

const int MAX_HOSTNAME = 200;
const int MAX_CONNECTIONS = 5;
const int MAX_RECV = 16384; //2 ^ 14

class Socket
{
    public:
        Socket();
        ~Socket();
        Socket(const Socket& other) {}

        // Server initialization
        bool create();
        bool bind(int port);
        bool listen() const;
        bool accept(Socket& socket) const;

        // Client initialization
        bool connect(const std::string& host, int port);

        // Data Transimission
        bool send(const std::string& msg) const;
        long recv(std::string&) const;

        void setNonBlocking(bool blocking);
        bool isValid() const;

    private:
        int sock_;
        sockaddr_in address_;
};

#endif
