
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



bool containsPyonFooter(const std::string& string)
{
    return string.find("---") != std::string::npos;
}



bool containsEndOfMessage(const std::string& string)
{
    return containsPyonFooter(string) && string.find("---\nPyON") == std::string::npos;
}



int main(int argc, char **argv)
{
    try
    {
        ClientSocket socket("localhost", 36330);

        std::string response;
        socket >> response;

        //socket << "updates add 0 1 $heartbeat\n";
        socket << "updates add 0 5 $(trajectory 0)\n";

        std::hash<std::string> hasher;

        while (true)
        {
            std::cout << "Reading... " << std::endl;
            std::string pyon;
            int trailerCount = 0;
            bool printed = false;

            while (true)
            {
                std::string buffer;
                socket >> buffer;
                pyon += buffer;

                if (!printed)
                {
                    std::cout << "read from socket" << std::endl;
                    printed = true;
                }

                if (containsPyonFooter(buffer))
                    trailerCount++;

                if (containsEndOfMessage(buffer))
                    break;
            }

            std::cout << pyon.length() << ", "
                      << hasher(pyon) << ", "
                      << trailerCount << std::endl;

            std::chrono::milliseconds duration(5000);
            std::this_thread::sleep_for(duration);
        }
    }
    catch (SocketException& e)
    {
        std::cout << "Exception was caught:" << e.description() << "\n";
    }
}
