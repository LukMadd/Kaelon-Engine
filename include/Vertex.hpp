#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

namespace renderer{
    struct Vertex{
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;
        
        static VkVertexInputBindingDescription getBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();

        void createVertexBuffer(VkBuffer &buffer, VkDeviceMemory &bufferMemory, VkCommandPool commandPool, VkQueue &graphicsQueue);
        void createIndexBuffer(VkBuffer &buffer, VkDeviceMemory &bufferMemory, VkCommandPool commandPool, VkQueue &graphicsQueue,  const std::vector<uint16_t> indices);
    }; 
}