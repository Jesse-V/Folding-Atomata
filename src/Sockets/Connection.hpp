
#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "ClientSocket.hpp"
#include <string>
#include <memory>

class Connection
{
    public:
        Connection(const std::string& host, int port);
        std::shared_ptr<ClientSocket> createClientSocket();
        std::string getHost();
        int getPort();

    private:
        std::string host_;
        int port_;
};

#endif
