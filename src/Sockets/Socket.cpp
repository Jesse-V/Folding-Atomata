
#include "Socket.hpp"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>


Socket::Socket() :
    sock_(-1)
{
    memset (&address_, 0, sizeof(address_));
}



Socket::~Socket()
{
    if (isValid())
        ::close(sock_);
}



bool Socket::create()
{
    sock_ = socket(AF_INET, SOCK_STREAM, 0);

    if (!isValid())
        return false;

    // TIME_WAIT - argh
    int on = 1;
    return setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on)) != -1;
}



bool Socket::bind(int port)
{
    if (!isValid())
        return false;

    address_.sin_family = AF_INET;
    address_.sin_addr.s_addr = INADDR_ANY;
    address_.sin_port = htons (port);

    int bind_return = ::bind(sock_, (struct sockaddr *)&address_, sizeof(address_));

    return bind_return != -1;
}



bool Socket::listen() const
{
    if (!isValid())
        return false;

    int listen_return = ::listen(sock_, MAXCONNECTIONS);
    return listen_return != -1;
}



bool Socket::accept(Socket& new_socket) const
{
    int addr_length = sizeof (address_);
    new_socket.sock_ = ::accept(sock_, (sockaddr*)&address_, (socklen_t*)&addr_length);

    return new_socket.sock_ > 0;
}



bool Socket::send(const std::string& msg) const
{
    int status = ::send(sock_, msg.c_str(), msg.size(), MSG_NOSIGNAL);
    return status != -1;
}



int Socket::recv(std::string& s) const
{
    char buf[MAXRECV + 1];
    memset ( buf, 0, MAXRECV + 1 );

    s = "";

    int status = ::recv(sock_, buf, MAXRECV, 0);

    if (status == -1)
    {
        std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
        return 0;
    }
    else if (status == 0)
    {
        return 0;
    }
    else
    {
        s = buf;
        return status;
    }
}



bool Socket::connect(const std::string& host, int port)
{
    if (!isValid())
        return false;

    address_.sin_family = AF_INET;
    address_.sin_port = htons(port);

    int status = inet_pton(AF_INET, host.c_str(), &address_.sin_addr);

    if (errno == EAFNOSUPPORT)
        return false;

    status = ::connect(sock_, (sockaddr*)&address_, sizeof(address_));
    return status == 0;
}



void Socket::setNonBlocking(bool b)
{
    int opts;
    opts = fcntl(sock_, F_GETFL);

    if (opts < 0)
        return;

    if (b)
        opts = (opts | O_NONBLOCK);
    else
        opts = (opts & ~O_NONBLOCK);

    fcntl(sock_, F_SETFL, opts);
}



bool Socket::isValid() const
{
     return sock_ != -1;
}
