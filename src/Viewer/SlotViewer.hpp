
#ifndef SLOT_VIEWER_HPP
#define SLOT_VIEWER_HPP

#include "../Sockets/Connection.hpp"
#include "../Sockets/ClientSocket.hpp"
#include "../PyON/TrajectoryParser.hpp"

/*
SlotViewer handles the viewing of the protein from a particular slot.
The protein is displayed as soon as the topology comes in, and then explodes
into position once the first checkpoint becomes available. From there, it iterates
through all checkpoints, and then jumps to the first checkpoint and repeats.
If Tracers are enabled, it shows them.
*/

class SlotViewer
{
    public:
        SlotViewer(const Connection& connection, int slotID);
        TrajectoryPtr getTrajectory();
        void update();
        void render();

    private:
        TrajectoryPtr loadTrajectory(const ClientSocket& socket, int slot);
        void addSnapshot();

        bool containsPyonFooter(const std::string& str);
        bool containsEndOfMessage(const std::string& str);

    private:
        Connection connection_;
        TrajectoryPtr trajectory_;
        int slotID_;
};

#endif
