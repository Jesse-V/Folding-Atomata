
#include "Snapshot.hpp"


void Snapshot::addPosition(const glm::vec3& position)
{
    positions_.push_back(position);
}



glm::vec3 Snapshot::getPosition(int atomIndex)
{
    return positions_[(std::size_t)atomIndex];
}
