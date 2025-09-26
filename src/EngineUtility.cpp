#include "EngineUtility.hpp"
#include <utility>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtx/string_cast.hpp>

namespace EngineUtility{
       bool rayIntersectsAABB(
        const glm::vec3& rayOrigin,
        const glm::vec3& rayDir,
        const glm::vec3& boxMin,
        const glm::vec3& boxMax,
        float& t
    ){
        const float EPSILON = 1e-6f;
        //"axis" for debugging
        auto updateSlab = [&](float rayO, float rayD, float bMin, float bMax, float &t0, float &t1, char axis) -> bool {
                if(fabs(rayD) < EPSILON){
                    if (rayO < bMin || rayO > bMax) return false;
                    return true;
                }
                float tNear = (bMin - rayO) / rayD;
                float tFar  = (bMax - rayO) / rayD;
                if (tNear > tFar) std::swap(tNear, tFar);
                t0 = std::max(t0, tNear);
                t1 = std::min(t1, tFar);
                return t0 <= t1;
            };

        float tMin = 0.0f;
        float tMax = FLT_MAX;

        if(!updateSlab(rayOrigin.x, rayDir.x, boxMin.x, boxMax.x, tMin, tMax, 'X')) return false;
        if(!updateSlab(rayOrigin.y, rayDir.y, boxMin.y, boxMax.y, tMin, tMax, 'Y')) return false;
        if(!updateSlab(rayOrigin.z, rayDir.z, boxMin.z, boxMax.z, tMin, tMax, 'Z')) return false; 

        t = tMin;

        glm::vec3 intersection = rayOrigin + rayDir * t;
        return true;
    }
}