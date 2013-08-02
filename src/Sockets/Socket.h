
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

class Socket
{
    public:
        Socket();
        virtual ~Socket();

        // Server initialization
        bool create();
        bool bind(int port);
        bool listen() const;
        bool accept(Socket& socket) const;

        // Client initialization
        bool connect(const std::string& host, int port);

        // Data Transimission
        bool send(const std::string& msg) const;
        int recv(std::string&) const;

        void setNonBlocking(bool blocking);
        bool isValid() const;

    private:
        int m_sock;
        sockaddr_in m_addr;
};


#endif
