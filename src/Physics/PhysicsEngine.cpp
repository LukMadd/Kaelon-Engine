#include "Physics/PhysicsEngine.hpp"
#include "ECS/Components.hpp"
#include "Physics/Collision.hpp"
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
    void PhysicsEngine::init(EngineScene::SceneManager *sceneManager, 
                             EnginePartitioning::Spatial_Partitioner *spatialPartitioner,
                             ECS* ecs){
        this->sceneManager = sceneManager;
        this->spatialPartitioner = spatialPartitioner;
        this->ecs = ecs;
    }
  
    void PhysicsEngine::tick(float deltaTime){
        tickCount++;

        std::unordered_set<std::pair<std::string, std::string>, PairHash> checkedPairs;
        for(auto& changedBoundingBox : changedBoundingBoxes){
            spatialPartitioner->updateEntityCells(changedBoundingBox);
        }

        for(auto entity : ecs->view<TransformComponent, BoundingBoxComponent, PhysicsComponent, SpatialPartitioningComponent>()){
            auto* transform = ecs->getComponent<TransformComponent>(entity);
            auto* boundingBox = ecs->getComponent<BoundingBoxComponent>(entity);
            auto* physics = ecs->getComponent<PhysicsComponent>(entity);
            auto* spatial = ecs->getComponent<SpatialPartitioningComponent>(entity);
            auto* metadata = ecs->getComponent<MetadataComponent>(entity);

            if(!physics->isStatic){
                //Applies gravity
                physics->velocity += (gravity * deltaTime) / 10.0f;
                if(physics->velocity.y >= 10.0f){
                    physics->velocity.y = 10.0f;
                }
            }
            glm::vec3 movement = physics->isStatic ? glm::vec3(0.0f) : physics->velocity * deltaTime;
            glm::vec3 newPos = transform->position;

            AABB predictedBox = boundingBox->worldBoundingBox;
            predictedBox.min += movement;
            predictedBox.max += movement;

            glm::vec3 currentMins = {boundingBox->worldBoundingBox.min.x, boundingBox->worldBoundingBox.min.y, boundingBox->worldBoundingBox.min.z};
            glm::vec3 currentMaxes = {boundingBox->worldBoundingBox.max.x, boundingBox->worldBoundingBox.max.y, boundingBox->worldBoundingBox.max.z};

            std::vector<Cell*> cells = spatialPartitioner->getCells(entity);
            for(int i = 0; i < cells.size(); i++){
                for(auto other : cells[i]->entities){
                    if(entity == other) continue;

                    BoundingBoxComponent otherBoundingBox = *ecs->getComponent<BoundingBoxComponent>(other);

                    std::pair<std::string, std::string> dirKey{metadata->uuid, ecs->getComponent<MetadataComponent>(other)->uuid};
                    if(!checkedPairs.insert(dirKey).second) continue;

                    float epsilon = 0.0000001f;

                    int collisionDirection = getCollisionDirection(predictedBox, otherBoundingBox.worldBoundingBox, currentMins, currentMaxes);
                    if(collisionDirection != OBJECT_COLLISION_NONE){
                        handleCollision(predictedBox, otherBoundingBox.worldBoundingBox, physics->velocity, 
                                      movement, currentMins, currentMaxes, collisionDirection, epsilon);
                    }

                }
            } 
        newPos += glm::vec3(movement.x, movement.y, movement.z);
        if(newPos != transform->position && !physics->isStatic){
            move(newPos, entity, *ecs);
        }
        }
    }
}