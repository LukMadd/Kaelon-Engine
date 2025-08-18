#include <vulkan/vulkan.hpp>
#include "MipMap.hpp"

namespace renderer{
    struct TextureLoader{
        void createTextureImage(VkImage &textureImage, MipMap &mipMap, VkDeviceMemory&textureImageMemory, VkCommandPool commandPool);
        void createTextureImageView(VkImage textureImage, VkImageView &textureImageView, MipMap mipMap);
        void createTextureSampler(VkSampler &textureSampler, uint32_t mipLevels);
    };
}


