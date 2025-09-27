#include "Object.hpp"
#include <iostream>

namespace EngineObject{
    void Object::createBoundingBox(){
        for(const auto& v : this->mesh->vertexBuffer.vertices){
            this->localBoundingBox.min.x = std::min(this->localBoundingBox.min.x, v.pos.x);
            this->localBoundingBox.min.y = std::min(this->localBoundingBox.min.y, v.pos.y);
            this->localBoundingBox.min.z = std::min(this->localBoundingBox.min.z, v.pos.z);

            this->localBoundingBox.max.x = std::max(this->localBoundingBox.max.x, v.pos.x);
            this->localBoundingBox.max.y = std::max(this->localBoundingBox.max.y, v.pos.y);
            this->localBoundingBox.max.z = std::max(this->localBoundingBox.max.z, v.pos.z);    
        }

        glm::mat4 model = glm::translate(glm::mat4(1.0f), this->node->transform.position)
                * glm::mat4_cast(this->node->transform.rotation)
                * glm::scale(glm::mat4(1.0f), this->node->transform.scale);

        //This feels visually overwhelming, I might actually have to change this
        glm::vec3 corners[8];
        corners[0] = glm::vec3(this->localBoundingBox.min.x, this->localBoundingBox.min.y, this->localBoundingBox.min.z);
        corners[1] = glm::vec3(this->localBoundingBox.max.x, this->localBoundingBox.min.y, this->localBoundingBox.min.z);
        corners[2] = glm::vec3(this->localBoundingBox.min.x, this->localBoundingBox.max.y, this->localBoundingBox.min.z);
        corners[3] = glm::vec3(this->localBoundingBox.max.x, this->localBoundingBox.max.y, this->localBoundingBox.min.z);
        corners[4] = glm::vec3(this->localBoundingBox.min.x, this->localBoundingBox.min.y, this->localBoundingBox.max.z);
        corners[5] = glm::vec3(this->localBoundingBox.max.x, this->localBoundingBox.min.y, this->localBoundingBox.max.z);
        corners[6] = glm::vec3(this->localBoundingBox.min.x, this->localBoundingBox.max.y, this->localBoundingBox.max.z);
        corners[7] = glm::vec3(this->localBoundingBox.max.x, this->localBoundingBox.max.y, this->localBoundingBox.max.z);

        glm::vec3 worldMin(FLT_MAX), worldMax(-FLT_MAX);
        for(int i = 0 ; i < 8; i++){
            glm::vec3 transformed  = glm::vec3(model * glm::vec4(corners[i], 1.0f));
            worldMin = glm::min(worldMin, transformed);
            worldMax = glm::max(worldMax, transformed);
        }

        this->worldBoundingBox.min = worldMin;
        this->worldBoundingBox.max = worldMax;
    }

    void Object::move(glm::vec3 position){
        this->node->transform.position = position;
        this->modelMatrix = glm::translate(glm::mat4(1.0f), position);
        createBoundingBox();
    }

    void Object::rotate(glm::vec3 rotation){
        if(node){
            node->transform.rotation.x = rotation.x;
            node->transform.rotation.y = rotation.y;
            node->transform.rotation.z = rotation.z;
            createBoundingBox();
        } else{
            std::cerr << "Failed to rotate object: Invalid/Uninitialized node" << std::endl;
        }
    }

    void Object::scale(glm::vec3 scale){
        if(node){
            node->transform.scale.x = scale.x;
            node->transform.scale.y = scale.y;
            node->transform.scale.z = scale.z;
            createBoundingBox();
        } else{
            std::cerr << "Failed to scale object: Invalid/Uninitialized node" << std::endl;
        }
    }
}