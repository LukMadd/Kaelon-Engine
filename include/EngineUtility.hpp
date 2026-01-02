#include <glm/glm.hpp>

namespace EngineUtility{
    bool rayIntersectsAABB(
        const glm::vec3& rayOrigin,
        const glm::vec3& rayDir,
        const glm::vec3& boxMin,
        const glm::vec3& boxMax,
        float& t
    );

    void initDebugSubSystems();
}