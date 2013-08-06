
#include "SlotViewer.hpp"
#include <thread>
#include <sstream>
#include <stdexcept>
#include <iostream>


SlotViewer::SlotViewer(const Connection& connection, int slotID) :
    connection_(connection), slotID_(slotID)
{
    std::cout << "Establishing new connection with FAHClient... ";
    auto socket = *connection.createClientSocket();

    std::string response;
    socket >> response;

    if (response.find("Welcome") == std::string::npos)
        throw std::runtime_error("Invalid response from FAHClient");

    std::cout << "done. Got good response back." << std::endl;
    std::cout << "Asking for trajectory... ";
    
    std::stringstream stream("");
    stream << "updates add 0 5 $(trajectory " << slotID << ")" << std::endl;
    socket << stream.str();

    std::cout << " done." << std::endl;

    trajectory_ = loadTrajectory(socket, slotID);

    std::thread snapshotAdder([&]() {
        std::cout << "New thread, awaiting incoming snapshots from " << 
            connection.getHost() << ":" << connection.getPort() << ":" <<
            slotID << " ... " << std::endl;
    });
}



TrajectoryPtr SlotViewer::getTrajectory()
{
    return trajectory_;
}



TrajectoryPtr SlotViewer::loadTrajectory(const ClientSocket& socket, int slotID)
{
    std::cout << "Reading trajectory response... ";

    std::string pyon;
    while (true)
    {
        std::string buffer;
        socket >> buffer;
        pyon += buffer;

        if (containsEndOfMessage(buffer))
            break;
    }

    std::cout << " done (" << pyon.length() << ")" << std::endl;

    TrajectoryParser parser;
    return parser.parse(pyon);
}



void SlotViewer::addSnapshot()
{

}



bool SlotViewer::containsPyonFooter(const std::string& str)
{
    return str.find("---") != std::string::npos;
}



bool SlotViewer::containsEndOfMessage(const std::string& str)
{
    return containsPyonFooter(str) && str.find("---\nPyON") == std::string::npos;
}
