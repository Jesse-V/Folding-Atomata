
#ifndef SNAPSHOT_HPP
#define SNAPSHOT_HPP

#include "glm/glm.hpp"
#include <vector>
#include <memory>

class Snapshot
{
    public:
        void addPosition(const glm::vec3& position);
        glm::vec3 getPosition(std::size_t atomIndex);

    private:
        std::vector<glm::vec3> positions_;
};

typedef std::shared_ptr<Snapshot> SnapshotPtr;

#endif
