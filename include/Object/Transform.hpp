#ifndef _TRANSFORM_HPP
#define _TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace EngineObject{
    struct Transform{
        glm::vec3 position{0.0f};
        glm::quat rotation{1,0,0,0};
        glm::vec3 scale{1.0f};
    
        glm::mat4 localMatrix{1.0f};
        glm::mat4 worldMatrix{1.0f};

        void updateMatrix(const glm::mat4& parentMatrix);
    };
}

#endif