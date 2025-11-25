#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "Misc/BoundingBox.hpp"

enum ObjectCollisionDirections{
    OBJECT_COLLISION_NONE,
    OBJECT_COLLISION_BELOW,
    OBJECT_COLLISION_ABOVE,
    OBJECT_COLLISION_LEFT,
    OBJECT_COLLISION_RIGHT,
    OBJECT_COLLISION_BEHIND,
    OBJECT_COLLISION_FRONT
};

namespace EnginePhysics{
    bool checkCollision(const AABB &predictedBox, const AABB &b);

    int getCollisionDirection(const AABB& predictedBox, const AABB &b, glm::vec3 currentMins, glm::vec3 currentMaxes);

    void handleCollision(AABB &predictedBoundingBox, const AABB &otherBoundingBox, glm::vec3& velocity, glm::vec3& movement, glm::vec3& currentMins, 
                         glm::vec3& currentMaxes, int objectCollisionDirection, float epsilon);
}

#endif