#include <vulkan/vulkan.hpp>
#include "Core/RecourseManager.hpp"
#include "Texture.hpp"

namespace EngineObject{
    struct DefaultResources{
        std::shared_ptr<Texture> init(EngineResource::ResourceManager &resourceManager);

        std::shared_ptr<Texture> texture;

        void cleanupDefault();

        bool isInitialized = false;
    };

    inline DefaultResources defaultResources;
}
