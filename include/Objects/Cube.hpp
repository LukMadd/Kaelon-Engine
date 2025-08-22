#include "Object.hpp"

using namespace EngineScene;

namespace EngineObject{
    class Cube : public Object{
        public:
            Cube(glm::vec3 position = {0.0f, 0.0f, 0.0f}, std::string modelPath = "", std::string texturePath = "");

            void initVulkanRecourses() override;
            void draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) override;
    };
}