#include "Cube.hpp"
#include "TextureLoader.hpp"
#include "ModelLoader.hpp"

namespace EngineObject{
    Cube::Cube(glm::vec3 position, std::string modelPath, std::string texturePath){
        this->mesh.meshPath = modelPath;
        this->texture.texturePath = texturePath;
        this->position = position;
        modelMatrix = glm::translate(glm::mat4(1.0f), position);
    }

    void Cube::initVulkanRecourses(){
        if(!mesh.meshPath.empty()) {
            mesh = ModelLoader::loadModel(mesh.meshPath);
        }

        if(!texture.texturePath.empty()){
            TextureLoader::createTextureImage(texture.texturePath, texture.textureImage, texture.textureImageMemory);
            TextureLoader::createTextureImageView(texture.textureImage, texture.textureImageView);
            TextureLoader::createTextureSampler(texture.textureSampler);
        } else{
            for(auto& v : mesh.vertexBuffer.vertices){
                v.color = { 1.0f, 0.5f, 0.25f };
            }
        }   
    }

    void Cube::draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout){
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &mesh.vertexBuffer.buffer, offsets);
        vkCmdBindIndexBuffer(commandBuffer, mesh.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
        
        vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &modelMatrix);

        vkCmdDrawIndexed(commandBuffer, mesh.indexCount, 1, 0, 0, 0);
    }
}