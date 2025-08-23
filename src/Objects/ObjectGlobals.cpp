#include "ObjectGlobals.hpp"
#include "RendererGlobals.hpp"
#include "RecourseManager.hpp"

using namespace EngineRenderer;

namespace EngineObject{
    std::shared_ptr<Texture> DefaultResources::init(EngineRecourse::RecourseManager &recourseManager){
        texture  = recourseManager.loadTexture("textures/viking_room.png");

        isInitialized = true;

        return texture;
    }

    void DefaultResources::cleanupDefault(){
        vkDestroySampler(device, texture->textureSampler, nullptr);
        vkDestroyImageView(device, texture->textureImageView, nullptr);
        vkDestroyImage(device, texture->textureImage, nullptr);
    }
}