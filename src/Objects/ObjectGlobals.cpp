#include "ObjectGlobals.hpp"
#include "RendererGlobals.hpp"
#include "RecourseManager.hpp"
#include "ValidationLayers.hpp"
#include <cstdint>

using namespace EngineRenderer;

namespace EngineObject{
    std::shared_ptr<Texture> DefaultResources::init(EngineResource::ResourceManager &resourceManager){
        texture = resourceManager.load<Texture>("viking_room.png");
        setObjectName(device, (uint64_t)texture->textureImage, VK_OBJECT_TYPE_IMAGE, "Default_Image");
        setObjectName(device, (uint64_t)texture->textureImageMemory, VK_OBJECT_TYPE_DEVICE_MEMORY, "Default_Image_Memory");
        setObjectName(device, (uint64_t)texture->textureImageView, VK_OBJECT_TYPE_IMAGE_VIEW, "Default_Image_View");
        setObjectName(device, (uint64_t)texture->textureSampler, VK_OBJECT_TYPE_SAMPLER, "Default_Texture_Sampler");

        isInitialized = true;

        return texture;
    }

    void DefaultResources::cleanupDefault(){
        vkDestroySampler(device, texture->textureSampler, nullptr);
        vkDestroyImageView(device, texture->textureImageView, nullptr);
        vkDestroyImage(device, texture->textureImage, nullptr);
        vkFreeMemory(device, texture->textureImageMemory, nullptr);
    }
}