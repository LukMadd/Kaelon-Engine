#ifndef AABB_HPP
#define AABB_HPP

#include "glm/glm.hpp"

struct AABB{
    glm::vec3 min = glm::vec3(FLT_MAX);
    glm::vec3 max = glm::vec3(-FLT_MAX);

    bool isInitialized = false;
};

#endif