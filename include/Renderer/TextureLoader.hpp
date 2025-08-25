#ifndef _TEXTURE_LOADER_HPP
#define _TEXTURE_LOADER_HPP

#include <vulkan/vulkan.hpp>

#include "Texture.hpp"


using namespace EngineObject;
namespace EngineRenderer{
    struct TextureLoader{
        static Texture createTexture(const std::string &filePath);
        static void createTextureImage(std::string texturePath, VkImage &textureImage, VkDeviceMemory&textureImageMemory);
        static void createTextureImageView(VkImage textureImage, VkImageView &textureImageView);
        static void createTextureSampler(VkSampler &textureSampler, const std::string &texturePath);
    };
}

#endif