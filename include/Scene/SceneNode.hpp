#ifndef _SCENE_NODE_HPP
#define _SCENE_NODE_HPP

#include "ECS/Components.hpp"
#include "ECS/ECS.hpp"

namespace EngineScene{
    
    class SceneNode{
        public:    
            void addChild(SceneNodeComponent& childNode, Entity parent);
            void removeChild(SceneNodeComponent& childNode);
            void update(Entity e, const glm::mat4& parentPos, ECS* ecs);
            glm::vec3 getWorldPosition(Entity e) const;

    };
}

#endif