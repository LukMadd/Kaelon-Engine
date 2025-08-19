#ifndef VERTEX_HPP
#define VERTEX_HPP

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <vulkan/vulkan.hpp>

namespace EngineRenderer{
    struct Vertex{
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;
        
        static VkVertexInputBindingDescription getBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();

        void createVertexBuffer(VkBuffer &buffer, VkDeviceMemory &bufferMemory, VkCommandPool commandPool, VkQueue &graphicsQueue);
        void createIndexBuffer(VkBuffer &buffer, VkDeviceMemory &bufferMemory, VkCommandPool commandPool, VkQueue &graphicsQueue);

        bool operator==(const Vertex &other) const{
            return pos == other.pos && color == other.color && texCoord == other.texCoord;
        }
    }; 
}


//I have NO idea what this does, but it works
namespace std {
    template<> struct hash<EngineRenderer::Vertex> {
        size_t operator()(EngineRenderer::Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                   (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                   (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

#endif