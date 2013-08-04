
#ifndef SLOT_VIEWER_HPP
#define SLOT_VIEWER_HPP

#include "../Sockets/ClientSocket.hpp"
#include "../PyON/TrajectoryParser.hpp"

class SlotViewer
{
    public:
        SlotViewer(const ClientSocket& socket, int slotID);
        TrajectoryPtr getTrajectory();

        static std::vector<SlotViewer> viewAllSlots(const std::string& host, int port);

    private:
        TrajectoryPtr loadTrajectory(const ClientSocket& socket);
        void addSnapshot();

        bool containsPyonFooter(const std::string& str);
        bool containsEndOfMessage(const std::string& str);

    private:
        ClientSocket socket_;
        TrajectoryPtr trajectory_;
        int slotIndex_;
};

#endif
