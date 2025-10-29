#include "Physics/PhysicsEngine.hpp"
#include "Scene/SceneManager.hpp"
#include <cassert>
#include <unordered_set>

enum ObjectCollisionDirections{
    OBJECT_COLLISION_NONE,
    OBJECT_COLLISION_BELOW,
    OBJECT_COLLISION_ABOVE,
    OBJECT_COLLISION_LEFT,
    OBJECT_COLLISION_RIGHT
};

struct PairHash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const noexcept {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

using namespace EnginePartitioning;

namespace EnginePhysics{
    void PhysicsEngine::init(SceneManager *sceneManager, 
                             EnginePartitioning::Spacial_Partitioner *spatialPartitioner){
        this->sceneManager = sceneManager;
        this->spatialPartitioner = spatialPartitioner;
    }

    bool PhysicsEngine::checkAABBCollision(const AABB &a, const AABB &b){
        return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
            (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
            (a.min.z <= b.max.z && a.max.z >= b.min.z);
    }

    int PhysicsEngine::checkAABBObjectCollisionYAxis(const AABB &a, const AABB &b, 
                                                    float previousMinY, float previousMaxY, 
                                                    float epsilon){
        bool horizontalOverlap = (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
                                (a.min.z <= b.max.z && a.max.z >= b.min.z);

        if(horizontalOverlap == false){
            return OBJECT_COLLISION_NONE;
        }


        if((previousMinY >= b.max.y - epsilon) && (a.min.y <= b.max.y + epsilon)){
            return OBJECT_COLLISION_BELOW;
        } 
        if((previousMaxY <= b.min.y + epsilon) && (a.max.y >= b.min.y - epsilon)){
            return OBJECT_COLLISION_ABOVE;
        }

        return OBJECT_COLLISION_NONE;
    }

    int PhysicsEngine::checkAABBObjectCollisionXAxis(const AABB &a, const AABB &b, 
                                                    float previousMinX, float previousMaxX, 
                                                    float epsilon){
        bool verticalOverlap = (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
                               (a.min.z <= b.max.z && a.max.z >= b.min.z);

        if(verticalOverlap == false){
            return OBJECT_COLLISION_NONE;
        }

        if((previousMinX >= b.max.x - epsilon) && (a.min.x <= b.max.x + epsilon)){
            return OBJECT_COLLISION_LEFT;
        }
        else if((previousMaxX <= b.min.x + epsilon) && (a.max.x >= b.min.x - epsilon)){
            return OBJECT_COLLISION_RIGHT;
        }

        return OBJECT_COLLISION_NONE;           
    }

    void handleCollisionYAxis(AABB &objectBoundingBox, const AABB &otherBoundingBox, 
                          float &y_Velocity, float &y_Movement, float &previousMinY, 
                          float &previousMaxY, int objectCollisionDirectionY, float epilson){
        float penetration;
        if(objectCollisionDirectionY == OBJECT_COLLISION_BELOW){
            penetration = otherBoundingBox.max.y - objectBoundingBox.min.y;
        } else if(objectCollisionDirectionY == OBJECT_COLLISION_ABOVE){
            penetration = objectBoundingBox.max.y - otherBoundingBox.min.y;
        }

        if(penetration > 0.0f){         
            if(objectCollisionDirectionY == OBJECT_COLLISION_BELOW){           
                objectBoundingBox.min.y += (penetration + epilson);
                objectBoundingBox.max.y += (penetration + epilson);
            } else if(objectCollisionDirectionY == OBJECT_COLLISION_ABOVE){
                objectBoundingBox.min.y -= (penetration + epilson);
                objectBoundingBox.max.y -= (penetration + epilson);
            }
            
            if(penetration > epilson){
                y_Velocity = 0.0f;
                y_Movement = 0.0f;
            }

            previousMinY = objectBoundingBox.min.y;
            previousMaxY = objectBoundingBox.max.y;
        }
    }

    void PhysicsEngine::tick(float deltaTime){
        tickCount++;

        std::unordered_set<std::pair<std::string, std::string>, PairHash> checkedPairs;

        for(auto &object : sceneManager->getCurrentScene()->objects){
            if(!object->isStatic){
                //Applies gravity
                object->velocity += (gravity * deltaTime) / 10.0f;
                if(object->velocity.y >= 10.0f){
                    object->velocity.y = 10.0f;
                }
            }
            glm::vec3 movement = object->isStatic ? glm::vec3(0.0f) : object->velocity * deltaTime;
            glm::vec3 newPos = object->getPosition();

            AABB predictedBox = object->worldBoundingBox;
            predictedBox.min += movement;
            predictedBox.max += movement;
            
            float previousMinY = object->worldBoundingBox.min.y;
            float previousMaxY = object->worldBoundingBox.max.y;

            std::vector<Cell*> cells = spatialPartitioner->getCells(object.get());
            for(int i = 0; i < cells.size(); i++){
                for(auto *other : cells[i]->objects){
                    if(object.get() == other) continue;

                    std::pair<std::string, std::string> dirKey{object->uuid, other->uuid};
                    if(!checkedPairs.insert(dirKey).second) continue;

                    float epsilon = 0.00001f;
                    if(!checkAABBCollision(predictedBox, other->worldBoundingBox)){
                        continue;
                    }

                    int collisionDirectionY = checkAABBObjectCollisionYAxis(predictedBox, other->worldBoundingBox, previousMinY, previousMaxY, epsilon);
                    if(collisionDirectionY != OBJECT_COLLISION_NONE){
                        handleCollisionYAxis(predictedBox, 
                            other->worldBoundingBox, object->velocity.y, movement.y, 
                                           previousMinY, previousMaxY, collisionDirectionY, 
                                     epsilon);
                    }

                }
            } 
        newPos += glm::vec3(movement.x, movement.y, movement.z);
        if(newPos != object->getPosition() && !object->isStatic){
            object->move(newPos);
        }
        }
    }
}