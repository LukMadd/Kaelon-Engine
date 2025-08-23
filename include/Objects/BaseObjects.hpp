#include "Object.hpp"
#include "ObjectGlobals.hpp"
#include "nlohmann/json.hpp"

using namespace EngineScene;

namespace EngineObject{
    class MeshObject : public Object{
        public:
            MeshObject(glm::vec3 position = {0.0f, 0.0f, 0.0f}, std::shared_ptr<Mesh> mesh = nullptr, std::shared_ptr<Texture> texture = nullptr);

            bool usingDefault = false;
            void initVulkanRecourses(EngineRecourse::RecourseManager &recourseManager) override;
            void draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) override;
            void deserialize(const nlohmann::json& jsonData) override;
            void cleanup(VkDevice device) override;

            std::string pendingMeshPath;
            std::string pendingTexturePath;
    };
}