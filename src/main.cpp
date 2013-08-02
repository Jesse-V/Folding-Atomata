
#include "main.hpp"
#include "Sockets/ClientSocket.hpp"
#include "Sockets/SocketException.hpp"
#include <thread>
#include <iostream>
#include <unordered_map>

/*
    Useful socket links:
    http://www.codeproject.com/Questions/368114/telnet-within-a-C-Cplusplus-program
    http://www.cpp-home.com/forum/viewtopic.php?t=8936
    http://codebase.eu/tutorial/linux-socket-programming-c/
    http://stackoverflow.com/questions/2952733/using-sys-socket-h-functions-on-windows
*/

int main(int argc, char **argv)
{
    try
    {
        ClientSocket socket("localhost", 36330);

        std::string response;
        socket >> response;

        //socket << "updates add 0 1 $heartbeat\n";
        socket << "trajectory 0\n";

        std::hash<std::string> hasher;

        std::string pyon;
        int trailerCount = 0;

        while (true)
        {
            std::string buffer;
            socket >> buffer;
            pyon += buffer;

            if (buffer.find("---") != std::string::npos)
                trailerCount++;

            if (buffer.find("---\n>") != std::string::npos)
                break;
        }

        std::cout << pyon.length() << ", "
                  << hasher(pyon) << ", "
                  << trailerCount << std::endl;

    }
    catch (SocketException& e)
    {
        std::cout << "Exception was caught:" << e.description() << "\n";
    }
}
