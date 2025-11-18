#ifndef _TRANSFORM_HPP
#define _TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace EngineObject{
    void updateMatrix(const glm::mat4& parentMatrix);
}

#endif