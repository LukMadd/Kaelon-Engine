#include "Scene/SceneNode.hpp"
#include "ECS/Components.hpp"

namespace EngineScene{
    void SceneNode::addChild(SceneNodeComponent& childNode, Entity parent){
        childNode.parent = parent;
    }

    void SceneNode::removeChild(SceneNodeComponent& childNode){
        childNode.parent = nullEntity;
    }

    void updateMatrix(TransformComponent& transform, const glm::mat4& parentMatrix = glm::mat4(1.0f)){
        transform.localMatrix = glm::translate(glm::mat4(1.0f), transform.position)
            * glm::mat4_cast(transform.rotation)
            * glm::scale(glm::mat4(1.0f), transform.scale);

        transform.worldMatrix = parentMatrix * transform.localMatrix;
    }

    void SceneNode::update(Entity e, const glm::mat4& parentMatrix, ECS& ecs){
        auto* t = ecs.getComponent<TransformComponent>(e); 
        if(!t) return;
        updateMatrix(*t, parentMatrix);
        for(auto& child : ecs.view<SceneNodeComponent>()){
            auto* node = ecs.getComponent<SceneNodeComponent>(child);
            if(node->parent == e){
                update(child, t->worldMatrix, ecs);
            }
        }
    }
}