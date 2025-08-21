#include <vulkan/vulkan.hpp>
#include "MipMap.hpp"

namespace EngineRenderer{
    struct TextureLoader{
        void createTextureImage(VkImage &textureImage, MipMap &mipMap, VkDeviceMemory&textureImageMemory);
        void createTextureImageView(VkImage textureImage, VkImageView &textureImageView, MipMap mipMap);
        void createTextureSampler(VkSampler &textureSampler, uint32_t mipLevels);
    };
}


