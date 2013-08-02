
#include "main.hpp"
#include <thread>
#include <iostream>


#include "Sockets/ClientSocket.h"
#include "Sockets/SocketException.h"

/*
    Useful socket links:
    http://www.codeproject.com/Questions/368114/telnet-within-a-C-Cplusplus-program
    http://www.cpp-home.com/forum/viewtopic.php?t=8936
    http://codebase.eu/tutorial/linux-socket-programming-c/
    http://linuxgazette.net/74/tougher.html
*/

int main(int argc, char **argv)
{
    try
    {
        ClientSocket socket("localhost", 36330);

        std::string reply;

        socket >> reply;
        std::cout << reply << std::endl;
        socket << "updates add 0 1 $heartbeat\n";

        while (true) //keep reading
        {
            socket >> reply;
            std::cout << reply << std::endl;

            std::chrono::milliseconds duration(500); //query every 500ms
            std::this_thread::sleep_for(duration); //C++11 way to sleep!
        }

    }
    catch (SocketException& e)
    {
        std::cout << "Exception was caught:" << e.description() << "\n";
    }
}
