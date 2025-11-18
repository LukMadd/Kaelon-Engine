#ifndef AABB_HPP
#define AABB_HPP

#include "glm/glm.hpp"

struct AABB{
    glm::vec3 min;
    glm::vec3 max;

    bool isInitialized = false;

    AABB()
    : min(FLT_MAX, FLT_MAX, FLT_MAX),
        max(-FLT_MAX, -FLT_MAX, -FLT_MAX),
        isInitialized(false) {}
};

#endif