#include "BaseObjects.hpp"
#include "RecourseManager.hpp"
#include "ObjectRegistry.hpp"

namespace EngineObject{
    Object* makeMeshObject(const nlohmann::json& data) {
        MeshObject* meshObject = new MeshObject();
        meshObject->deserialize(data);
        return meshObject;
    }

    static bool registered = []() {
        ObjectRegistry::get().registerType("Mesh_Object", makeMeshObject);
        return true;
    }();

    void MeshObject::deserialize(const nlohmann::json& jsonData){
    pendingMeshPath = jsonData["mesh"];
    pendingTexturePath = jsonData["texture"];

    name = jsonData["name"];

    shader.vertShader = jsonData["shader"]["vert"];
    shader.fragShader = jsonData["shader"]["frag"];

    modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
    }

    MeshObject::MeshObject(glm::vec3 position, const std::string &meshPathRef, const std::string &texturePathRef){
        this->mesh = std::make_shared<Mesh>();
        this->texture = std::make_shared<Texture>();

        this->name = "Mesh_Object";
        this->type = "Mesh_Object";
        this->mesh->meshPath = meshPathRef;
        this->texture->texturePath = texturePathRef;
        this->position = position;
        modelMatrix = glm::translate(glm::mat4(1.0f), position);
    }

    void MeshObject::initVulkanResources(EngineResource::ResourceManager &resourceManager){
        if(!pendingMeshPath.empty()){
            mesh = resourceManager.load<Mesh>(pendingMeshPath);
        } else if(mesh && !mesh->meshPath.empty()){
            mesh = resourceManager.load<Mesh>(mesh->meshPath);
        }

        if(!pendingTexturePath.empty()){
            texture = resourceManager.load<Texture>(pendingTexturePath);
        } else if(texture && !texture->texturePath.empty()){
            texture = resourceManager.load<Texture>(texture->texturePath);
        }else{
            texture = defaultResources.texture;
        }
    }

    void MeshObject::draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout){
        VkDeviceSize offsets[] = {0};

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &mesh->vertexBuffer.buffer, offsets);
        vkCmdBindIndexBuffer(commandBuffer, mesh->indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
        
        vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &modelMatrix);

        vkCmdDrawIndexed(commandBuffer, mesh->indexCount, 1, 0, 0, 0);
    }

    void MeshObject::cleanup(VkDevice device){
        if(mesh->vertexBuffer.buffer){
            vkDestroyBuffer(device, mesh->vertexBuffer.buffer, nullptr);
            mesh->vertexBuffer.buffer = VK_NULL_HANDLE;
        }
        if(mesh->vertexBuffer.bufferMemory){
            vkFreeMemory(device, mesh->vertexBuffer.bufferMemory, nullptr);
            mesh->vertexBuffer.bufferMemory = VK_NULL_HANDLE;
        }

        if(mesh->indexBuffer.buffer){
            vkDestroyBuffer(device, mesh->indexBuffer.buffer, nullptr);
            mesh->indexBuffer.buffer = VK_NULL_HANDLE;
        }
        if(mesh->indexBuffer.bufferMemory){
            vkFreeMemory(device, mesh->indexBuffer.bufferMemory, nullptr);
            mesh->indexBuffer.bufferMemory = VK_NULL_HANDLE;
        }
        
        if(!usingDefault){
            if(texture->textureSampler){
                vkDestroySampler(device, texture->textureSampler, nullptr);
                texture->textureSampler = VK_NULL_HANDLE;
            }
            if(texture->textureImageView){
                vkDestroyImageView(device, texture->textureImageView, nullptr);
                texture->textureImageView = VK_NULL_HANDLE;
            }
            if(texture->textureImage){
                vkDestroyImage(device, texture->textureImage, nullptr);
                texture->textureImage = VK_NULL_HANDLE;
            }
            if(texture->textureImageMemory){
                vkFreeMemory(device, texture->textureImageMemory, nullptr);
                texture->textureImageMemory = VK_NULL_HANDLE;
            }
        } 
    }
}