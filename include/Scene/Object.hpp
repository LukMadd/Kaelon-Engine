#ifndef _OBJECT_HPP
#define _OBJECT_HPP

#include "Mesh.hpp"
#include "nlohmann/json.hpp"
#include "RecourseManager.hpp"
#include "UUID.hpp"

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

using namespace EngineRenderer;
using namespace EngineObject;

namespace EngineScene{
    struct Shader{
        std::string vertShader = ("shaders/base_vert_shader.spv");
        std::string fragShader = ("shaders/base_frag_shader.spv");
    };

    class Object{
        public:
            Object() : uuid(generateUUID()){};
            virtual ~Object() = default;
            virtual void draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) = 0;
            virtual void initVulkanRecourses(EngineRecourse::RecourseManager &recourseManager) = 0;
            virtual void deserialize(const nlohmann::json& jsonData) = 0;
            virtual void cleanup(VkDevice device) = 0;
            std::string type;
            std::string name;

            std::string uuid;
            glm::mat4 modelMatrix{1.0f};
            glm::vec3 position;
            VkDescriptorSetLayout descriptorSetLayout;
            std::vector<VkDescriptorSet> descriptorSets;

            std::shared_ptr<Mesh> mesh;
            Shader shader;
            std::shared_ptr<Texture> texture;
    };
}

#endif