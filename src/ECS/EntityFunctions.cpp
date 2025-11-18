#include "ECS/EntityFunctions.hpp"
#include "Debug/Debugger.hpp"
#include "ECS/Components.hpp"
#include "Misc/Material.hpp"
#include "Misc/UUID.hpp"
#include "ECS/ECS.hpp"
#include "EngineGlobals.hpp"

#include "Spatial/Spatial_Partitioner.hpp"
#include <cstdint>

Entity initEntity(ECS& ecs, std::string meshPath, std::string texturePath, std::string name, std::string type){
    uint32_t entityID = ecs.getAvailableEntityID();
    Entity entity = entityID;
    ecs.addEntity(entity);
    ecs.addComponent<TransformComponent>(entity);
    
    auto* metadataComponent = ecs.addComponent<MetadataComponent>(entity);
    metadataComponent->uuid = generateUUID();
    if(!name.empty()){
        metadataComponent->name = name;
    }
    if(!type.empty()){
        metadataComponent->type = type;
    }

    if(!meshPath.empty()){
        MeshComponent* meshComponent = ecs.addComponent<MeshComponent>(entity);
        meshComponent->mesh->meshPath = meshPath;
    }

    MaterialComponent* materialComponent = ecs.addComponent<MaterialComponent>(entity);
    materialComponent->material = std::make_shared<Material>();
    if(!texturePath.empty()){
        materialComponent->material->addPendingTexture(texturePath);
    }

    return entity;
}

void initResources(Entity e, ECS& ecs, EngineResource::ResourceManager &resourceManager,
                          EnginePartitioning::Spatial_Partitioner *spatialPartitioner){
    auto* entity_mesh = ecs.getComponent<MeshComponent>(e);
    if(!entity_mesh->mesh->meshPath.empty()){
        entity_mesh->mesh = resourceManager.load<Mesh>(entity_mesh->mesh->meshPath);
    }

    auto* entity_material = ecs.getComponent<MaterialComponent>(e);
    if(!entity_material->material->getPendingTextures().empty()){
        entity_material->material->getTextures().clear();
        for(const auto& texturePath : entity_material->material->getPendingTextures()){
            auto texture = resourceManager.load<Texture>(texturePath);
            if(texture){
                entity_material->material->addTexture(texture);
                entity_material->hasTexture = true;
            }
        }
    } else{
        for(auto& texture : entity_material->material->getTextures()){
            if(texture->texturePath.empty()){
                continue;
            }
            if(!texture->textureSampler || !texture->textureImage){
                texture = resourceManager.load<Texture>(texture->texturePath);
            }
        }
    }

    if(ecs.hasComponent<SpatialPartitioningComponent>(e)){
        spatialPartitioner->addEntity(e);
    }
}

void draw(Entity e, ECS& ecs, VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout){
    if(!ecs.hasComponent<MeshComponent>(e)){
        DEBUGGER_LOG(WARNING, "Invalid entity for drawing, no mesh component!", "Renderer");
        return;
    } 

    auto* entity_transform = ecs.getComponent<TransformComponent>(e);
    auto* entity_mesh = ecs.getComponent<MeshComponent>(e);
    VkDeviceSize offsets[] = {0};   

    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &entity_mesh->mesh->vertexBuffer.buffer, offsets);
    vkCmdBindIndexBuffer(commandBuffer, entity_mesh->mesh->indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::mat4), &entity_transform->modelMatrix);

    vkCmdDrawIndexed(commandBuffer, entity_mesh->mesh->indexCount, 1, 0, 0, 0);
}

void createBoundingBox(Entity e, ECS& ecs){
    auto* entity_mesh = ecs.getComponent<MeshComponent>(e);
    auto* entity_transform = ecs.getComponent<TransformComponent>(e);
    auto* entity_AABB = ecs.getComponent<BoundingBoxComponent>(e);

    for(const auto& v : entity_mesh->mesh->vertexBuffer.vertices){
        entity_AABB->localBoundingBox.min = glm::min(entity_AABB->localBoundingBox.min, v.pos);
        entity_AABB->localBoundingBox.max = glm::max(entity_AABB->localBoundingBox.max, v.pos);
    }

    glm::mat4 model = glm::translate(glm::mat4(1.0f), entity_transform->position)
        * glm::mat4_cast(entity_transform->rotation)
        * glm::scale(glm::mat4(1.0f), entity_transform->scale);

    glm::vec3 corners[8];
    corners[0] = glm::vec3(entity_AABB->localBoundingBox.min.x, entity_AABB->localBoundingBox.min.y, entity_AABB->localBoundingBox.min.z);
    corners[1] = glm::vec3(entity_AABB->localBoundingBox.max.x, entity_AABB->localBoundingBox.min.y, entity_AABB->localBoundingBox.min.z);
    corners[2] = glm::vec3(entity_AABB->localBoundingBox.min.x, entity_AABB->localBoundingBox.max.y, entity_AABB->localBoundingBox.min.z);
    corners[3] = glm::vec3(entity_AABB->localBoundingBox.max.x, entity_AABB->localBoundingBox.max.y, entity_AABB->localBoundingBox.min.z);
    corners[4] = glm::vec3(entity_AABB->localBoundingBox.min.x, entity_AABB->localBoundingBox.min.y, entity_AABB->localBoundingBox.max.z);
    corners[5] = glm::vec3(entity_AABB->localBoundingBox.max.x, entity_AABB->localBoundingBox.min.y, entity_AABB->localBoundingBox.max.z);
    corners[6] = glm::vec3(entity_AABB->localBoundingBox.min.x, entity_AABB->localBoundingBox.max.y, entity_AABB->localBoundingBox.max.z);
    corners[7] = glm::vec3(entity_AABB->localBoundingBox.max.x, entity_AABB->localBoundingBox.max.y, entity_AABB->localBoundingBox.max.z);

    glm::vec3 worldMin(FLT_MAX), worldMax(-FLT_MAX);
    for(int i = 0 ; i < 8; i++){
        glm::vec3 transformed  = glm::vec3(model * glm::vec4(corners[i], 1.0f));
        worldMin = glm::min(worldMin, transformed);
        worldMax = glm::max(worldMax, transformed);
    }

    entity_AABB->worldBoundingBox.min = worldMin;
    entity_AABB->worldBoundingBox.max = worldMax;

    entity_AABB->worldBoundingBox.isInitialized = true;

    EngineGlobals::changedBoundingBoxes.push_back(e);
}

void move(glm::vec3 position, Entity e, ECS& ecs){
    auto* transform = ecs.getComponent<TransformComponent>(e);
    transform->position = position;

    glm::mat4 model = glm::translate(glm::mat4(1.0f), transform->position);
    model *= glm::mat4_cast(transform->rotation);
    model = glm::scale(model, transform->scale);

    transform->modelMatrix = model;
    createBoundingBox(e, ecs);
}

void rotate(glm::quat rotation, Entity e, ECS& ecs){
    auto* transform = ecs.getComponent<TransformComponent>(e);
    transform->rotation = glm::quat(rotation);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), transform->position);
    model *= glm::mat4_cast(transform->rotation);
    model = glm::scale(model, transform->scale);

    transform->modelMatrix = model;
    createBoundingBox(e, ecs);
}

void scale(glm::vec3 scale, Entity e, ECS& ecs){
    auto* transform = ecs.getComponent<TransformComponent>(e);
    transform->scale.x = scale.x;
    transform->scale.y = scale.y;
    transform->scale.y = scale.y;

    glm::mat4 model = glm::translate(glm::mat4(1.0f), transform->position);
    model *= glm::mat4_cast(transform->rotation);
    model = glm::scale(model, transform->scale);

    transform->modelMatrix = model;
    createBoundingBox(e, ecs);
}
