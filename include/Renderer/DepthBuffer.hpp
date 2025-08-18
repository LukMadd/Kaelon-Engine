#ifndef _DEPTH_BUFFER_HPP
#define _DEPTH_BUFFER_HPP

#include <vulkan/vulkan.hpp>

namespace renderer{
    struct DepthBuffer{
        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;

        void createDepthResources(VkCommandPool commandPool, VkExtent2D swapChainExtent, VkImage &depthImage, VkDeviceMemory &depthImageMemory, VkImageView &depthImageView);
        VkFormat findDepthFormat();
        void cleanup();
    };
}

#endif