
#include "FAHClientIO.hpp"
#include "SlotViewer.hpp"
#include "../PyON/StringManip.hpp"
#include <sstream>
#include <stdexcept>
#include <iostream>


FAHClientIO::FAHClientIO(const std::shared_ptr<ClientSocket> socket) :
    socket_(socket)
{
    connectToFAHClient();
}



void FAHClientIO::connectToFAHClient()
{
    std::cout << "Connecting to local FAHClient... ";
    std::string response = SlotViewer::readResponse(*socket_);

    if (response.find("Welcome") == std::string::npos)
        throw std::runtime_error("Invalid response from FAHClient");

    std::cout << "done. Got good response back." << std::endl;
}



int FAHClientIO::getSlotCount()
{
    std::cout << "Determining number of slots... ";
    *socket_ << "num-slots\n";

    std::string begin = "PyON 1 num-slots", end = "---";
    std::string nSlotsStr = SlotViewer::readResponse(*socket_);
    std::stringstream stream(StringManip::between(nSlotsStr, begin, end));

    int nSlots;
    stream >> nSlots;
    std::cout << nSlots << std::endl;
    return nSlots;
}



std::vector<TrajectoryPtr> FAHClientIO::getTrajectories()
{
    std::vector<TrajectoryPtr> trajectories;
    int nSlots = getSlotCount();
    for (int slotIndex = 0; slotIndex < nSlots; slotIndex++)
    {
        std::cout << "Downloading trajectory for slot " << slotIndex << "... ";

        std::stringstream trajectoryRequest("");
        trajectoryRequest << "trajectory " << slotIndex << std::endl;
        *socket_ << trajectoryRequest.str();

        std::string trajectoryStr = SlotViewer::readResponse(*socket_);
        std::cout << "done." << std::endl;

        if (trajectoryStr.find("\"atoms\": []\"") == std::string::npos)
            trajectories.push_back(TrajectoryParser::parse(trajectoryStr));
    }

    std::cout << "Filtered out FAHCore 17 slots, now have " << 
                           trajectories.size() << " trajectories." << std::endl;

    return trajectories;
}
