#include <glm/glm.hpp>
#include "ECS/Components.hpp"
#include "Spatial/Spatial_Partitioner.hpp"

using namespace EnginePartitioning;

struct Ray{
    glm::vec3 origin;
    glm::vec3 direction;
};

struct RayCastHit{
    Entity entity;
    float distance; 
    glm::vec3 point;
    glm::vec3 normal;
};

//Need to absract away the spatial partioner and ecs into a struct or something
bool raycast(RayCastHit& hit, Ray ray, float maxDistance, Spatial_Partitioner* spatial_partitioner, ECS* ecs);