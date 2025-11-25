#include "Physics/Collision.hpp"

namespace EnginePhysics{
    bool checkCollision(const AABB &predictedBox, const AABB &b){
        if(predictedBox.max.x < b.min.x || predictedBox.min.x > b.max.x) return false;
        if(predictedBox.max.y < b.min.y || predictedBox.min.y > b.max.y) return false;
        if(predictedBox.max.z < b.min.z || predictedBox.min.z > b.max.z) return false;

        return true;
     }

    //Due to predicted bounding boxes(the one being passed into this function) you cannot use just the a's bounding box for checks,
    //instead you must get recieved its current mins and maxs, not what it will be.
    int getCollisionDirection(const AABB& predictedBox, const AABB &b, glm::vec3 currentMins, glm::vec3 currentMaxes){
        float epsilon = 0.00001f;

        bool horizontalOverlap = (predictedBox.min.x <= b.max.x && predictedBox.max.x >= b.min.x) &&
                                 (predictedBox.min.z <= b.max.z && predictedBox.max.z >= b.min.z);
        
        bool verticalOverlap = (predictedBox.min.y <= b.max.y && predictedBox.max.y >= b.min.y) &&
                               (predictedBox.min.z <= b.max.z && predictedBox.max.z >= b.min.z);
        
        if(!horizontalOverlap && !verticalOverlap){
            return OBJECT_COLLISION_NONE;
        }

        //currentMins[0] and currentMaxs[0] is X
        if((currentMins[0] >= b.max.x - epsilon) && (predictedBox.min.x <= b.max.x + epsilon)){
            return OBJECT_COLLISION_LEFT;
        }
        else if((currentMaxes[0] <= b.min.x + epsilon) && (predictedBox.max.x >= b.min.x - epsilon)){
            return OBJECT_COLLISION_RIGHT;
        }

        else if((currentMins[1] >= b.max.y - epsilon) && (predictedBox.min.y <= b.max.y + epsilon)){
            return OBJECT_COLLISION_BELOW;
        } 
        else if((currentMaxes[1] <= b.min.y + epsilon) && (predictedBox.max.y >= b.min.y - epsilon)){
            return OBJECT_COLLISION_ABOVE;
        }

        else if((currentMins[2] <= b.max.z - epsilon) && (predictedBox.min.z <= b.max.z + epsilon)){
            return OBJECT_COLLISION_BEHIND;
        } 
        else if((currentMaxes[2] <= b.min.z + epsilon) && (predictedBox.max.z <= b.min.z - epsilon)){
            return OBJECT_COLLISION_FRONT;
        }

        return OBJECT_COLLISION_NONE;
    }

    //velocity, movement, currentMins, currentMaxes = [0,1,2]->[x,y,z]
    void handleCollision(AABB &predictedBoundingBox, const AABB &otherBoundingBox, glm::vec3& velocity, glm::vec3& movement, glm::vec3& currentMins, 
                        glm::vec3& currentMaxes, int objectCollisionDirection, float epsilon){
        float penetration;
         if(objectCollisionDirection == OBJECT_COLLISION_LEFT){
            penetration = otherBoundingBox.max.x - predictedBoundingBox.min.x;
            if(penetration > epsilon){
                predictedBoundingBox.min.x += (penetration + epsilon);
                predictedBoundingBox.max.x += (penetration + epsilon);

                velocity[0] = 0.0f;
                movement[0] = 0.0f;
            }
        
        } else if(objectCollisionDirection == OBJECT_COLLISION_RIGHT){
            penetration = predictedBoundingBox.max.x - otherBoundingBox.min.x;
            if(penetration > epsilon){
                predictedBoundingBox.min.x -= (penetration + epsilon);
                predictedBoundingBox.max.x -= (penetration + epsilon);

                velocity[0] = 0.0f;
                movement[0] = 0.0f;
            }
        
        } else if(objectCollisionDirection == OBJECT_COLLISION_BELOW){
            penetration = otherBoundingBox.max.y - predictedBoundingBox.min.y;
            if(penetration > epsilon){
                predictedBoundingBox.min.y += (penetration + epsilon);
                predictedBoundingBox.max.y += (penetration + epsilon);

                velocity[1] = 0.0f;
                movement[1] = 0.0f;
            }

        } else if(objectCollisionDirection == OBJECT_COLLISION_ABOVE){
            penetration = predictedBoundingBox.max.y - otherBoundingBox.min.y;
            if(penetration > epsilon){
                predictedBoundingBox.min.y -= (penetration + epsilon);
                predictedBoundingBox.max.y -= (penetration + epsilon);

                velocity[1] = 0.0f;
                movement[1] = 0.0f;
            }
        
        } else if(objectCollisionDirection == OBJECT_COLLISION_BEHIND){
            penetration = otherBoundingBox.max.z - predictedBoundingBox.min.z;
            if(penetration > epsilon){
                predictedBoundingBox.min.z += (penetration + epsilon);
                predictedBoundingBox.max.z += (penetration + epsilon);

                velocity[2] = 0.0f;
                movement[2] = 0.0f;
            }
        
        } else if(objectCollisionDirection == OBJECT_COLLISION_FRONT){
            penetration = predictedBoundingBox.max.z - otherBoundingBox.min.z;
            if(penetration > epsilon){
                predictedBoundingBox.min.z -= (penetration + epsilon);
                predictedBoundingBox.max.z -= (penetration + epsilon);

                velocity[2] = 0.0f;
                movement[2] = 0.0f;
            }
        }

        //Sets the current bounding box to the predicted bouding box
        for(int i = 0; i < 3; i++){
            currentMins[i] = predictedBoundingBox.min[i];
            currentMaxes[i] = predictedBoundingBox.max[i];
        }

    }

}