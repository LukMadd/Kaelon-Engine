#ifndef _TEXTURE_LOADER_HPP
#define _TEXTURE_LOADER_HPP

#include <vulkan/vulkan.hpp>

namespace EngineRenderer{
    struct TextureLoader{
        static void createTextureImage(std::string texturePath, VkImage &textureImage, VkDeviceMemory&textureImageMemory);
        static void createTextureImageView(VkImage textureImage, VkImageView &textureImageView);
        static void createTextureSampler(VkSampler &textureSampler);
    };
}

#endif