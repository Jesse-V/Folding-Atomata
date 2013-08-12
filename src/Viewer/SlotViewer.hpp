
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
        ~SlotViewer();
        TrajectoryPtr getTrajectory();
        void addSnapshot(const SnapshotPtr& newSnapshot);
        void update();
        void render();

        static std::string readResponse(const ClientSocket& socket);

    private:
        void launchThread();
        TrajectoryPtr loadTrajectory(const ClientSocket& socket, int slot);
        void addIncomingSnapshots();

    private:
        std::shared_ptr<ClientSocket> socket_;
        TrajectoryPtr trajectory_;
        int slotID_;
};

#endif
