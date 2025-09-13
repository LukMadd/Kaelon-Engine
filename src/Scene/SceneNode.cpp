#include "SceneNode.hpp"
#include "Object.hpp"

namespace EngineScene{
    SceneNode::SceneNode(){
        transform.position = glm::vec3(0.0f);
        transform.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        transform.scale    = glm::vec3(1.0f);
        transform.localMatrix = glm::mat4(1.0f);
        transform.worldMatrix = glm::mat4(1.0f);
        object = nullptr;
    }

    void SceneNode::addChild(SceneNode *child){
        child->parent = this;
        children.push_back(child);
    }

    void SceneNode::removeChild(SceneNode* child){
        children.erase(std::remove(children.begin(), children.end(), child), children.end());
        child->parent = nullptr;
    }

    void SceneNode::update(const glm::mat4 &parentMatrix){
        transform.updateMatrix(parentMatrix);
        if(object) {
            object->modelMatrix = transform.worldMatrix;
        }
        for(auto* child : children) {
            child->update(transform.worldMatrix);
        }
    }

    glm::vec3 SceneNode::getWorldPosition() const{
        return glm::vec3(transform.worldMatrix[3]);
    }
}