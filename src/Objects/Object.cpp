#include "Object.hpp"

namespace EngineObject{
    void Object::createBoundingBox(Object *object){
        for(const auto& v : object->mesh->vertexBuffer.vertices){
            object->localBoundingBox.min.x = std::min(object->localBoundingBox.min.x, v.pos.x);
            object->localBoundingBox.min.y = std::min(object->localBoundingBox.min.y, v.pos.y);
            object->localBoundingBox.min.z = std::min(object->localBoundingBox.min.z, v.pos.z);

            object->localBoundingBox.max.x = std::max(object->localBoundingBox.max.x, v.pos.x);
            object->localBoundingBox.max.y = std::max(object->localBoundingBox.max.y, v.pos.y);
            object->localBoundingBox.max.z = std::max(object->localBoundingBox.max.z, v.pos.z);    
        }

        object->worldBoundingBox.min = object->getPosition() + object->localBoundingBox.min * object->node->transform.scale;
        object->worldBoundingBox.max = object->getPosition() + object->localBoundingBox.max * object->node->transform.scale;
    }

    void Object::move(glm::vec3 position){
        this->node->transform.position = position;
        this->modelMatrix = glm::translate(glm::mat4(1.0f), position);
        createBoundingBox(this);
    }
}