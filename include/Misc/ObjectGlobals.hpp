#include <vulkan/vulkan.hpp>
#include "Core/ResourceManager.hpp"
#include "Texture.hpp"

struct DefaultResources{
    std::shared_ptr<Texture> init(EngineResource::ResourceManager &resourceManager);

    std::shared_ptr<Texture> texture;

    void cleanupDefault();

    bool isInitialized = false;
};

inline DefaultResources defaultResources;

