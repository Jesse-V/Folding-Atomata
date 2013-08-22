
#include "Snapshot.hpp"
#include <sstream>
#include <stdexcept>


void Snapshot::addPosition(const glm::vec3& position)
{
    positions_.push_back(position);
}



glm::vec3 Snapshot::getPosition(int atomIndex)
{
    if (atomIndex < 0 || atomIndex >= (int)positions_.size())
    {
        std::stringstream stream("");
        stream << "Index " << atomIndex << " out of [0," << 
            positions_.size() << ") bounds!";
        throw std::runtime_error(stream.str());
    }

    return positions_[(std::size_t)atomIndex];
}
