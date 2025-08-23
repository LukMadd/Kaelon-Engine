#include <vulkan/vulkan.hpp>
#include "RecourseManager.hpp"
#include "Texture.hpp"

namespace EngineObject{
    struct DefaultResources{
        std::shared_ptr<Texture> init(EngineRecourse::RecourseManager &recourseManager);

        std::shared_ptr<Texture> texture;

        void cleanupDefault();

        bool isInitialized = false;
    };

    inline DefaultResources defaultResources;
}