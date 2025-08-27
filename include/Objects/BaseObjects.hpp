#include "Object.hpp"
#include "ObjectGlobals.hpp"
#include "nlohmann/json.hpp"

using namespace EngineScene;

namespace EngineObject{
    class MeshObject : public Object{
        public:
            MeshObject(glm::vec3 position = {0.0f, 0.0f, 0.0f}, const std::string &meshPathRef = "", const std::string &texturePathRef = "");

            bool usingDefault = false;
            void initVulkanResources(EngineResource::ResourceManager &resourceManager) override;
            void draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) override;
            void deserialize(const nlohmann::json& jsonData) override;
            void cleanup(VkDevice device) override;

            std::string pendingMeshPath = "";
            std::string pendingTexturePath = "";
    };
}