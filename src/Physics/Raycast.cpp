#include "Physics/Raycast.hpp"
#include "ECS/Components.hpp"
#include "Spatial/Spatial_Partitioner.hpp"
#include <unordered_set>

bool raycast(RayCastHit& hit, Ray ray, float maxDistance, Spatial_Partitioner* spatial_partitioner, ECS& ecs){
    ray.direction = glm::normalize(ray.direction);
    hit.normal = glm::vec3(0.0f);

    glm::vec3 rayEnd = ray.origin + glm::normalize(ray.direction) * maxDistance;
    BoundingBoxComponent rayAABB;
    rayAABB.worldBoundingBox.min = glm::min(ray.origin, rayEnd);
    rayAABB.worldBoundingBox.max = glm::max(ray.origin,rayEnd);

    std::unordered_set<Entity> possibleEntities;
    std::vector<Cell*> cells = spatial_partitioner->getCellsFromAABB(&rayAABB);
    for(auto& cell :cells){
        for(auto& entity : cell->entities){
            possibleEntities.insert(entity);
        }
    }

    bool hitFound = false;
    float closestHit = maxDistance;

    for(auto& entity : possibleEntities){
        if(!ecs.hasComponent<BoundingBoxComponent>(entity)) continue;

        auto* boundingBox = ecs.getComponent<BoundingBoxComponent>(entity);
        
        float tx1 = (boundingBox->worldBoundingBox.min.x - ray.origin.x) / ray.direction.x;
        float tx2 = (boundingBox->worldBoundingBox.max.x - ray.origin.x) / ray.direction.x;
        float txMin = std::min(tx1, tx2);
        float txMax = std::max(tx1, tx2);

        float ty1 = (boundingBox->worldBoundingBox.min.y - ray.origin.y) / ray.direction.y;
        float ty2 = (boundingBox->worldBoundingBox.max.y - ray.origin.y) / ray.direction.y;
        float tyMin = std::min(ty1, ty2);
        float tyMax = std::max(ty1, ty2);

        float tz1 = (boundingBox->worldBoundingBox.min.z - ray.origin.z) / ray.direction.z;
        float tz2 = (boundingBox->worldBoundingBox.max.z - ray.origin.z) / ray.direction.z;
        float tzMin = std::min(tz1, tz2);
        float tzMax = std::max(tz1, tz2);

        float tEntry = std::max({txMin, tyMin, tzMin});
        float tExit  = std::min({txMax, tyMax, tzMax});

        if(tEntry > tExit || tExit < 0.0f || tEntry > closestHit)
         continue; 

        //0: X, 1: Y, 2: Z
        int axis = 0;
        if(tyMin > tEntry) {tEntry = tyMin; axis = 1;}
        if(tzMin > tEntry) {tEntry = tzMin; axis = 2;}

        if(axis == 0) hit.normal.x = (ray.direction.x > 0.0f) ? -1.0f : 1.0f;
        else if(axis == 1) hit.normal.y = (ray.direction.y > 0.0f) ? -1.0f : 1.0f;
        else hit.normal.z = (ray.direction.z > 0.0f) ? -1.0f : 1.0f;

        closestHit = tEntry;
        hit.distance = tEntry;
        hit.point = ray.origin + ray.direction * tEntry;
        hit.entity = entity;

        hitFound = true;
    }
    return hitFound;
}