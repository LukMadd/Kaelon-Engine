#ifndef _ENTITY_FUNCTIONS_HPP
#define _ENTITY_FUNCTIONS_HPP

#include "Components.hpp"
#include "Core/ResourceManager.hpp"

namespace EnginePartitioning{
    class Spatial_Partitioner;
};

class ECS;

Entity initEntity(ECS& ecs, std::string meshPath = "", std::string texturePath = "", std::string name = "", std::string type = "");

void initResources(Entity e, ECS& ecs, EngineResource::ResourceManager &resourceManager,
                   EnginePartitioning::Spatial_Partitioner *SpatialPartitioner);

void draw(Entity e, ECS& ecs, VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout);

void createBoundingBox(Entity e, ECS& ecs);

void move(glm::vec3 position, Entity e, ECS& ecs);

void rotate(glm::quat rotation, Entity e, ECS& ecs);

void scale(glm::vec3 scale, Entity e, ECS& ecs);

#endif