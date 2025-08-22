#include "Object.hpp"
#include "Vertex.hpp"
#include "TextureLoader.hpp"
#include <cstdint>

namespace EngineScene{
    Quad::Quad(glm::vec3 position, std::string textureImagePath){
        mesh.vertexBuffer.vertices = {
            {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{ 0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}, 
            {{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}}, 
            {{-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        };

        mesh.indexBuffer.indices = {
        0, 1, 2,
        2, 3, 0
        };


        texture.texturePath = textureImagePath;

        modelMatrix = glm::mat4(1.0f);
        mesh.indexCount = static_cast<uint32_t>(mesh.indexBuffer.indices.size());

        modelMatrix = glm::translate(glm::mat4(1.0f), position);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void Quad::initVulkanRecourses(){
        mesh.vertexBuffer.createVertexBuffer(mesh.vertexBuffer.vertices ,mesh.vertexBuffer.buffer, mesh.vertexBuffer.bufferMemory);
        mesh.indexBuffer.createIndexBuffer(mesh.indexBuffer.indices ,mesh.indexBuffer.buffer, mesh.indexBuffer.bufferMemory);
        if(texture.texturePath != ""){
            TextureLoader::createTextureImage(texture.texturePath, texture.textureImage, texture.textureImageMemory);
            TextureLoader::createTextureImageView(texture.textureImage, texture.textureImageView);
            TextureLoader::createTextureSampler(texture.textureSampler);
        }
    }

    void Quad::draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout){
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &mesh.vertexBuffer.buffer, offsets);
        vkCmdBindIndexBuffer(commandBuffer, mesh.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
        
        vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &modelMatrix);

        vkCmdDrawIndexed(commandBuffer, mesh.indexCount, 1, 0, 0, 0);
    }

    bool Texture::isValid(){
        return textureImage && textureImageMemory && textureImageView && textureSampler;
    }

    Quad::~Quad() = default;
}