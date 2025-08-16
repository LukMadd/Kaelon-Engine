#include <vulkan/vulkan.hpp>

#include "Vertex.hpp"

namespace renderer{
    inline VkQueue graphicsQueue;
    inline VkQueue presentQueue;
    inline VkDevice device;
    inline VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    const std::string MODEL_PATH = "models/viking_room.obj";
    const std::string TEXTURE_PATH = "textures/viking_room.png";

    inline std::vector<Vertex> vertices;
    inline std::vector<uint32_t> indices;
}