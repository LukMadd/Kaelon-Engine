#ifndef _MIP_MAP_HPP
#define _MIP_MAP_HPP

#include <vulkan/vulkan.hpp>

namespace EngineRenderer{
    class MipMap{
        public:
            void generateMipMaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels, VkCommandPool commandPool);

            uint32_t mipLevels;
    };
}

#endif