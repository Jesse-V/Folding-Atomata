
#ifndef SOCKET_EXCEPTION_HPP
#define SOCKET_EXCEPTION_HPP

#include <string>

class SocketException
{
    public:
        SocketException(const std::string& description) :
            description_(description)
        {}

        std::string description()
        {
            return description_;
        }

    private:
        std::string description_;
};

#endif
