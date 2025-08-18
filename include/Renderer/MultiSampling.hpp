#ifndef _MULTI_SAMPLING_HPP
#define _MULTI_SAMPLING_HPP

#include <vulkan/vulkan.hpp>

namespace renderer{
    class MultiSampler{
        VkImage colorImage;
        VkDeviceMemory colorImageMemory;

        public:
            VkImageView colorImageView;

            void createColorResources(VkFormat swapChainImageFormat, VkExtent2D swapChainExtent);

            void cleanup();
    };
}

#endif