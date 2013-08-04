
#include "SlotViewer.hpp"
#include <thread>
#include <iostream>


SlotViewer::SlotViewer(const ClientSocket& socket) :
    socket_(socket)
{

}



static std::vector<SlotViewer> SlotViewer::viewAllSlots(const std::string& host, int port)
{
    ClientSocket socket(host, port);

    std::string response;
    socket >> response;
}



TrajectoryPtr SlotViewer::getTrajectory()
{
    return trajectory_;
}



TrajectoryPtr SlotViewer::loadTrajectory(const ClientSocket& socket)
{
    socket << "updates add 0 5 $(trajectory 0)\n";
    std::cout << "Reading... ";
    std::string pyon;

    while (true)
    {
        std::string buffer;
        socket >> buffer;
        pyon += buffer;

        if (containsEndOfMessage(buffer))
            break;
    }

    std::cout << "... done reading (" << pyon.length() << ")" << std::endl;

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
