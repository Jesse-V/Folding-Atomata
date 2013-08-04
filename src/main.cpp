
#include "main.hpp"
#include "Sockets/ClientSocket.hpp"
#include "Sockets/SocketException.hpp"
#include "PyON/TrajectoryParser.hpp"
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
    //slot-info, in between "\"id\": \"" and "\",\n"
    //1) check for heartbeat, throw error if no connection
    //2) get topology. This data should be accessible as soon it's available.
    //3) get checkpoints. Can view more and more of them as they come in.
    
    try
    {
        ClientSocket socket("localhost", 36330);

        std::string response;
        socket >> response;

        socket << "updates add 0 5 $(trajectory 0)\n";

        std::hash<std::string> hasher;

        while (true)
        {
            std::cout << "Reading... " << std::endl;
            std::string pyon;

            while (true)
            {
                std::string buffer;
                socket >> buffer;
                pyon += buffer;

                if (containsEndOfMessage(buffer))
                    break;
            }

            std::cout << pyon.length() << ", "
                      << hasher(pyon) << std::endl;

            TrajectoryParser parser;
            parser.parse(pyon);

            std::chrono::milliseconds duration(5000);
            std::this_thread::sleep_for(duration);
        }
    }
    catch (SocketException& e)
    {
        std::cout << "Exception was caught:" << e.description() << "\n";
    }
}
