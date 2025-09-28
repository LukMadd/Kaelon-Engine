#include "Physics/PhysicsEngine.hpp"
#include "Scene/SceneManager.hpp"
#include <cassert>

using namespace EnginePartitioning;


namespace EnginePhysics{
    void PhysicsEngine::init(SceneManager *sceneManager, 
                             EnginePartitioning::Spacial_Partitioner *spatialPartitioner){
        this->sceneManager = sceneManager;
        this->spatialPartitioner = spatialPartitioner;
    }

    void PhysicsEngine::applyGravity(Object *object){
        if(!object->isStoppedByGround && !object->isStatic){
            object->move(object->getPosition() + object->velocity);
        }
    }

    bool PhysicsEngine::checkAABCollision(const AAB &a, const AAB &b){
        return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
            (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
            (a.min.z <= b.max.z && a.max.z >= b.min.z);
    }

    void PhysicsEngine::tick(float deltaTime){
        tickCount++;

        for(auto &object : sceneManager->getCurrentScene()->objects){
            object->velocity = gravity * deltaTime;
            applyGravity(object.get());
            std::vector<Cell*> cells = spatialPartitioner->getCells(object.get());
            for(int i = 0; i < cells.size(); i++){
                for(int j = 0; j < cells[i]->objects.size(); j++){
                    if(object.get() == cells[i]->objects[j]){
                        continue;
                    }
                    if(checkAABCollision(object->worldBoundingBox, cells[i]->objects[j]->worldBoundingBox)){        
                        if(!object->isStatic) object->isStoppedByGround = true;
                    } else{
                        if(!object->isStatic) object->isStoppedByGround = false;
                    }
                }
            }   
        }
    }
}