
#ifndef FAHCLIENT_IO
#define FAHCLIENT_IO

#include "../Sockets/ClientSocket.hpp"
#include "../Trajectory/Trajectory.hpp"
#include <memory>
#include <vector>

class FAHClientIO
{
    public:
        FAHClientIO(const std::shared_ptr<ClientSocket> socket);
        int getSlotCount();
        std::vector<TrajectoryPtr> getTrajectories();

    private:
        void connectToFAHClient();

    private:
        std::shared_ptr<ClientSocket> socket_;
};

#endif
