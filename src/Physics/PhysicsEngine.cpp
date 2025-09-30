#include "Physics/PhysicsEngine.hpp"
#include "Scene/SceneManager.hpp"
#include <cassert>
#include <unordered_set>
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

    bool PhysicsEngine::checkAABCollision(const AAB &a, const AAB &b){
        return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
            (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
            (a.min.z <= b.max.z && a.max.z >= b.min.z);
    }

    bool PhysicsEngine::checkAABGroundUnderneath(const AAB &a, const AAB &b, 
                                                 float previousMinY, float epsilon){
        bool horizontalOverlap = (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
                                (a.min.z <= b.max.z && a.max.z >= b.min.z);


        bool crossed = (previousMinY >= b.max.y - epsilon) && (a.min.y <= b.max.y + epsilon);

        return horizontalOverlap && crossed;

    }

    void handleDownwardCollision(AAB &objectBoundingBox, const AAB &otherBoundingBox, 
                                float &y_Velocity, float &y_Movement, float &previousMinY){
        float offsetY = otherBoundingBox.max.y - objectBoundingBox.min.y;                        
        objectBoundingBox.min.y += offsetY;
        objectBoundingBox.max.y += offsetY;
        
        y_Velocity = 0.0f;
        y_Movement = 0.0f;

        previousMinY = objectBoundingBox.min.y;
    }

    void PhysicsEngine::tick(float deltaTime){
        tickCount++;

        std::unordered_set<std::pair<std::string, std::string>, PairHash> checkedPairs;

        for(auto &object : sceneManager->getCurrentScene()->objects){
            if(!object->isStatic){
                object->velocity += (gravity * deltaTime) / 10.0f;
                if(object->velocity.y >= 10.0f){
                    object->velocity.y = 10.0f;
                }
            }

            glm::vec3 movement = object->isStatic ? glm::vec3(0.0f) : object->velocity * deltaTime;
            glm::vec3 newPos = object->getPosition();

            AAB predictedBox = object->worldBoundingBox;
            predictedBox.min += movement;
            predictedBox.max += movement;
            
            float previousMinY = object->worldBoundingBox.min.y;

            std::vector<Cell*> cells = spatialPartitioner->getCells(object.get());
            for(int i = 0; i < cells.size(); i++){
                for(auto *other : cells[i]->objects){
                    if(object.get() == other) continue;

                    std::pair<std::string, std::string> dirKey{object->uuid, other->uuid};
                    if(!checkedPairs.insert(dirKey).second) continue;

                    float epsilon = glm::abs(movement.y) + 0.01f;
                    if(checkAABGroundUnderneath(predictedBox, other->worldBoundingBox, previousMinY, epsilon)){        
                        handleDownwardCollision(predictedBox, other->worldBoundingBox,
                                   object->velocity.y, movement.y, previousMinY);
                    }
                }
            } 
        newPos+=movement;
        if(newPos != object->getPosition() && !object->isStatic){
            object->move(newPos);
        }
        }
    }
}