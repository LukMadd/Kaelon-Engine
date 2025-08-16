#include <vulkan/vulkan.hpp>

namespace renderer{
    struct textureLoader{
        void createTextureImage(VkImage &textureImage, VkDeviceMemory&textureImageMemory, VkCommandPool commandPool);
        void createTextureImageView(VkImage textureImage, VkImageView &textureImageView);
        void createTextureSampler(VkSampler &textureSampler);
    };
}


