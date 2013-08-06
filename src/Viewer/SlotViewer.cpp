
#include "SlotViewer.hpp"
#include <thread>
#include <sstream>
#include <iostream>


SlotViewer::SlotViewer(const ClientSocket& socket, int slotID) :
    socket_(socket), slotID_(slotID)
{
    trajectory_ = loadTrajectory(socket, slotID);
}



TrajectoryPtr SlotViewer::getTrajectory()
{
    return trajectory_;
}



TrajectoryPtr SlotViewer::loadTrajectory(const ClientSocket& socket, int slotID)
{
    std::cout << "Asking for trajectory... ";

    std::stringstream stream("");
    stream << "updates add 0 5 $(trajectory " << slotID << ")" << std::endl;
    socket << stream.str();

    std::cout << " done." << std::endl;
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
