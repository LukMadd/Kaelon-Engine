#ifndef  _TEXTURE_HPP
#define _TEXTURE_HPP

#include <vulkan/vulkan.hpp>

namespace EngineObject{
    struct Texture{
        std::string texturePath;
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
        VkSampler textureSampler;

        bool isValid() {return textureImage && textureImageMemory && textureImageView && textureImageView && textureSampler;}
    };
}

#endif