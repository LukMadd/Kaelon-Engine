#include <vulkan/vulkan.hpp>
#include "Object.hpp"

namespace EngineObject{
    struct DefaultResources{
        void init();

        EngineScene::Texture texture;

        void cleanupDefault();

        bool isInitialized = false;
    };
}