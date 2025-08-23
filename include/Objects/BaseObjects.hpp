#include "Object.hpp"
#include "ObjectGlobals.hpp"
#include "nlohmann/json.hpp"

using namespace EngineScene;

namespace EngineObject{
    class MeshObject : public Object{
        public:
            MeshObject(glm::vec3 position = {0.0f, 0.0f, 0.0f}, std::string modelPath = "", std::string texturePath = "");

            DefaultResources defaultTexture;
            bool usingDefault = false;
            void initVulkanRecourses() override;
            void draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) override;
            void deserialize(const nlohmann::json& jsonData) override;
            void cleanup(VkDevice device) override;
    };
}