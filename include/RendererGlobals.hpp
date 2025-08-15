#include <vulkan/vulkan.hpp>

namespace renderer{
    inline VkQueue graphicsQueue;
    inline VkQueue presentQueue;
    inline VkDevice device;
    inline VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
}