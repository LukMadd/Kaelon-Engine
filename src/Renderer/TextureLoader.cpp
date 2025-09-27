#include <cstdint>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "TextureLoader.hpp"
#include "Vertex.hpp"
#include "RendererUtilities.hpp"
#include "Renderer.hpp"
#include "RendererGlobals.hpp"

namespace EngineRenderer{
    std::shared_ptr<Texture> TextureLoader::createTexture(const std::string &filePath){
        auto texture = std::make_shared<Texture>();
        
        texture->texturePath = filePath;
        createTextureImage(filePath, texture->textureImage, texture->textureImageMemory);
        createTextureImageView(texture->textureImage, texture->textureImageView);
        createTextureSampler(texture->textureSampler);

        return texture;
    }

    void TextureLoader::createTextureImage(std::string texturePath, VkImage &textureImage, VkDeviceMemory &textureImageMemory){
        Vertex vertex;

        int texWidth, texHeight, texChannels;
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* pixels = stbi_load((std::string(KAELON_TEXTURE_DIR) + texturePath).c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        if(!pixels){
            throw std::runtime_error("Failed to load texture image: " + std::string(KAELON_TEXTURE_DIR) + texturePath + "!");
        }
 
        mipMap.mipLevels = static_cast<uint32_t>(std::floor(std::log2((std::max(texWidth, texHeight))))) + 1;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        createBuffer("Texture_Buffer_Name_" + texturePath, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void *data;
        vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
            memcpy(data, pixels, static_cast<uint32_t>(imageSize));
        vkUnmapMemory(device, stagingBufferMemory);

        stbi_image_free(pixels);

        createImage(texWidth, texHeight, mipMap.mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

        transitionImageLayout(commandPool,textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipMap.mipLevels);
        copyBufferToImage(commandPool, stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

        mipMap.generateMipMaps(textureImage, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipMap.mipLevels, commandPool);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
    }

    void TextureLoader::createTextureImageView(VkImage textureImage, VkImageView &textureImageView){
        textureImageView = createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mipMap.mipLevels);
    }

    void TextureLoader::createTextureSampler(VkSampler &textureSampler, VkFilter filter){
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(physicalDevice, &properties);

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = filter;
        samplerInfo.minFilter = filter;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = VK_LOD_CLAMP_NONE;
        samplerInfo.mipLodBias = 0.0f;

        VkResult result = vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler);
        if(result != VK_SUCCESS){
            throw std::runtime_error("Failed to create texture sampler error code: " + std::to_string(result) + "!");
        }
        setObjectName(device, (uint64_t)textureSampler, VK_OBJECT_TYPE_SAMPLER, "Texture_Sampler(" + std::to_string((uint64_t)textureSampler) + ")");
    }
}
