#ifndef _QUEUE_HPP
#define _QUEUE_HPP

#include <cstdint>
#include <optional>
#include <vulkan/vulkan.hpp>

namespace renderer {
    struct QueueFamilyIndices{
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete();
    };

    class Queue{
        public:
            QueueFamilyIndices FindQueueFamily(VkPhysicalDevice device, VkSurfaceKHR surface);
    };
}

#endif