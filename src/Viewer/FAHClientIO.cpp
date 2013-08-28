
/******************************************************************************\
                     This file is part of Folding Atomata,
          a program that displays 3D views of Folding@home proteins.

                      Copyright (c) 2013, Jesse Victors

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see http://www.gnu.org/licenses/

                For information regarding this software email:
                                Jesse Victors
                         jvictors@jessevictors.com
\******************************************************************************/

#include "FAHClientIO.hpp"
#include "../PyON/TrajectoryParser.hpp"
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
    std::string response = readResponse();

    if (response.find("Welcome") == std::string::npos)
        throw std::runtime_error("Invalid response from FAHClient");

    std::cout << "done. Got good response back." << std::endl;
}



int FAHClientIO::getSlotCount()
{
    std::cout << "Determining number of slots... ";
    *socket_ << "num-slots\n";

    std::string begin = "PyON 1 num-slots", end = "---";
    std::string nSlotsStr = readResponse();
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

        std::string trajectoryStr = readResponse();
        std::cout << "done." << std::endl;

        if (trajectoryStr.find("\"atoms\": []") == std::string::npos)
            trajectories.push_back(TrajectoryParser::parse(trajectoryStr));
    }

    std::cout << "Filtered out FahCore 17 slots, left with " << 
                           trajectories.size() << " trajectories." << std::endl;

    return trajectories;
}



std::string FAHClientIO::readResponse()
{
    std::string pyon;

    while (true)
    {
        std::string buffer;
        *socket_ >> buffer;
        pyon += buffer;

        if (buffer.find("\n> ")      != std::string::npos ||
           (buffer.find("---")       != std::string::npos && 
            buffer.find("---\nPyON") == std::string::npos)
        )
            break; //reached end of message
    }

    return pyon;
}
