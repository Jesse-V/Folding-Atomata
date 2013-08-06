
#include "SlotViewer.hpp"
#include <thread>
#include <sstream>
#include <stdexcept>
#include <iostream>


SlotViewer::SlotViewer(const Connection& connection, int slotID) :
    connection_(connection), slotID_(slotID)
{
    std::stringstream idStream("");
    idStream << connection.getHost() << ":" << connection.getPort() << ":" << slotID;

    std::cout << "Establishing new connection with FAHClient... ";
    std::cout.flush();

    ClientSocket socket(connection.getHost(), connection.getPort());
    //auto socket = *connection.createClientSocket();

    std::string response = readResponse(socket);

    if (response.find("Welcome") == std::string::npos)
        throw std::runtime_error("Invalid response from FAHClient");

    std::cout << "done. Got good response back." << std::endl;
    std::cout << "Connection ID is " << idStream.str() << std::endl;
    std::cout << "Asking for trajectory... ";

    std::stringstream stream("");
    stream << "updates add 0 5 $(trajectory " << slotID << ")" << std::endl;
    socket << stream.str();

    std::cout << "done." << std::endl;

    trajectory_ = loadTrajectory(socket, slotID);

    std::thread snapshotAdder([&]() {
        std::cout << "New thread for " << idStream.str() << 
            ", awaiting incoming snapshots... " << std::endl;

        TrajectoryParser parser;
        while (true)
        {
            std::string positionsStr = readResponse(socket);
            std::cout << idStream.str() << "'s thread read new snapshot. Adding... ";
            addSnapshot(parser.parseSnapshot(positionsStr));
            std::cout << "done." << std::endl;

            std::chrono::milliseconds duration(5000);
            std::this_thread::sleep_for(duration);
        }
    });
    snapshotAdder.detach(); //run thread detached from main execution
}



TrajectoryPtr SlotViewer::getTrajectory()
{
    return trajectory_;
}



TrajectoryPtr SlotViewer::loadTrajectory(const ClientSocket& socket, int slotID)
{
    std::cout << "Reading trajectory response... ";
    std::string pyon = readResponse(socket);
    std::cout << " done (" << pyon.length() << ")" << std::endl;

    TrajectoryParser parser;
    return parser.parse(pyon);
}



void SlotViewer::addSnapshot(const SnapshotPtr& newSnapshot)
{
    trajectory_->addSnapshot(newSnapshot);
}



std::string SlotViewer::readResponse(const ClientSocket& socket)
{
    std::string pyon;

    while (true)
    {
        std::string buffer;
        socket >> buffer;
        pyon += buffer;

        if (buffer.find("\n> ")      != std::string::npos ||
           (buffer.find("---")       != std::string::npos && 
            buffer.find("---\nPyON") == std::string::npos)
        )
            break; //reached end of message
    }

    return pyon;
}
