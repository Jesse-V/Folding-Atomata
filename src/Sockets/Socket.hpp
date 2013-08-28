
/******************************************************************************\
                     This file is part of Folding Atomata,
          a program that displays 3D views of Folding@home proteins.

                      Copyright (c) 2013, Jesse Victors

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see http://www.gnu.org/licenses/

                For information regarding this software email:
                                Jesse Victors
                         jvictors@jessevictors.com
\******************************************************************************/

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
