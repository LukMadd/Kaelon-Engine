#include "Object/Transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>


namespace EngineObject{
    void Transform::updateMatrix(const glm::mat4& parentMatrix){
        localMatrix = glm::translate(glm::mat4(1.0f), position)
                    * glm::mat4_cast(rotation)
                    * glm::scale(glm::mat4(1.0f), scale);

        worldMatrix = parentMatrix * localMatrix;
    }
}