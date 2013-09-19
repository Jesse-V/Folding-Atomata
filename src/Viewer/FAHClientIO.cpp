
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
#include "PyON/TrajectoryParser.hpp"
#include "PyON/StringManip.hpp"
#include "Options.hpp"
#include <sstream>
#include <stdexcept>
#include <iostream>


FAHClientIO::FAHClientIO(const std::shared_ptr<ClientSocket>& socket) :
    socket_(socket)
{
    connectToFAHClient();
    authenticate();
}



void FAHClientIO::connectToFAHClient()
{
    std::cout << "Connecting to local FAHClient... ";
    std::string response = readResponse();

    if (response.find("Welcome") == std::string::npos)
        throw std::runtime_error("Invalid response from FAHClient");

    std::cout << "done. Got good response back." << std::endl;
}



void FAHClientIO::authenticate()
{
    if (!Options::getInstance().usesPassword())
        return;

    std::cout << "Authenticating to FAHClient... ";
    *socket_ << "auth " << Options::getInstance().getPassword() << "\n";
    std::string response = readResponse();

    if (response.find("OK") == std::string::npos)
        throw std::runtime_error(response);

    std::cout << "success!" << std::endl;
}



std::vector<int> FAHClientIO::getSlotIDs()
{
    std::cout << "Determining available slots... found { ";

    *socket_ << "slot-info\n";
    const std::string BEGIN = "\"id\":", END = ",\n";
    std::string slotInfoStr = readResponse();

    std::vector<int> slotIDs;
    std::size_t index = slotInfoStr.find(BEGIN, 0);
    while (index != std::string::npos)
    {
        auto value = StringManip::between(slotInfoStr, BEGIN, END, index);
        value = StringManip::trim(value, " \"");

        int id;
        std::istringstream(value) >> id;
        slotIDs.push_back(id);

        index = slotInfoStr.find(BEGIN, index + 1);
        std::cout << id << " ";
    }

    std::cout << "}" << std::endl;

    return slotIDs;
}



std::vector<TrajectoryPtr> FAHClientIO::getTrajectories()
{
    std::vector<TrajectoryPtr> trajectories;
    auto slotIDs = getSlotIDs();
    for (int id : slotIDs)
    {
        std::cout << "Downloading trajectory for slot " << id << "... ";
        std::cout.flush();

        std::stringstream trajectoryRequest("");
        trajectoryRequest << "trajectory " << id << std::endl;
        *socket_ << trajectoryRequest.str();

        std::string trajectoryStr = readResponse();
        std::cout << "done." << std::endl;

        if (trajectoryStr != "> " &&
            trajectoryStr.find("\"atoms\": []") == std::string::npos
        )
        {
            TrajectoryParser trajectoryParser(trajectoryStr);
            trajectories.push_back(trajectoryParser.parse());
        }
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
            pyon == "> "                                  ||
           (buffer.find("---")       != std::string::npos &&
            buffer.find("---\nPyON") == std::string::npos)
        )
            break; //reached end of message
    }

    return pyon;
}
