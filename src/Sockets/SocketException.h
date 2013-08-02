
#ifndef SOCKET_EXCEPTION_HPP
#define SOCKET_EXCEPTION_HPP

#include <string>

class SocketException
{
    public:
        SocketException(std::string s) : m_s(s) {}

        std::string description()
        {
            return m_s;
        }

    private:
        std::string m_s;
};

#endif
