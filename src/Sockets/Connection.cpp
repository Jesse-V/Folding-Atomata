
#include "Connection.hpp"


Connection::Connection(const std::string& host, int port) :
    host_(host), port_(port)
{}



std::shared_ptr<ClientSocket> Connection::createClientSocket()
{
    return std::make_shared<ClientSocket>("localhost", 36330);
}



std::string Connection::getHost()
{
    return host_;
}



int Connection::getPort()
{
    return port_;
}
