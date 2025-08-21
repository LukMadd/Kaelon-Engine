#include <vulkan/vulkan.hpp>

#include "Vertex.hpp"

namespace EngineRenderer{
    inline VkQueue graphicsQueue;
    inline VkQueue presentQueue;
    inline VkDevice device;
    inline VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    inline VkCommandPool commandPool;

    inline std::vector<Vertex> vertices;
    inline std::vector<uint32_t> indices;

    const std::string MODEL_PATH = "models/viking_room.obj";
    const std::string TEXTURE_PATH = "textures/viking_room.png";

    inline VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
}