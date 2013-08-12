
#include "SlotViewer.hpp"
#include <iostream>


SlotViewer::SlotViewer(const TrajectoryPtr& trajectory) :
    trajectory_(trajectory)
{}



SlotViewer::~SlotViewer()
{
    std::cout << "SlotViewer destructing..." << std::endl;
    std::cout.flush();
}



void SlotViewer::update()
{

}



void SlotViewer::render()
{

}








/* save this for https://github.com/Jesse-V/Folding-Atomata/issues/16
    std::thread snapshotAdder([]() {
        try
        {
            std::cout << "New thread for " << idStream.str() << 
                ", awaiting incoming snapshots... " << std::endl;

            TrajectoryParser parser;
            while (true)
            {
                std::string positionsStr = readResponse(*socket_);
                std::cout << idStream.str() << "'s thread read new snapshot. Adding... ";
                addSnapshot(parser.parseSnapshot(positionsStr));
                std::cout << "done." << std::endl;

                std::chrono::milliseconds duration(5000);
                std::this_thread::sleep_for(duration);
            }
        }
    catch (SocketException& e)
    {
        std::cout << "In SlotViewer's thread: " << e.description() << "\n";
    }
    });
    snapshotAdder.detach(); //run thread detached from main execution
    */
