#include "ObjectGlobals.hpp"
#include "TextureLoader.hpp"
#include "RendererGlobals.hpp"

using namespace EngineRenderer;

namespace EngineObject{
    void DefaultResources::init(){
        TextureLoader::createTextureImage("textures/viking_room.png",texture.textureImage, texture.textureImageMemory);
        TextureLoader::createTextureImageView(texture.textureImage, texture.textureImageView);
        TextureLoader::createTextureSampler(texture.textureSampler);

        isInitialized = true;
    }

    void DefaultResources::cleanupDefault(){
        vkDestroySampler(device, texture.textureSampler, nullptr);
        vkDestroyImageView(device, texture.textureImageView, nullptr);
        vkDestroyImage(device, texture.textureImage, nullptr);
    }
}