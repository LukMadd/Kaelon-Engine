#ifndef _OBJECT_HPP
#define _OBJECT_HPP

#include "Vertex.hpp"
#include "nlohmann/json.hpp"
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

#include <cstdint>

using namespace EngineRenderer;

namespace EngineScene{
    struct Mesh{
        std::string meshPath;
        VertexBuffer vertexBuffer;
        IndexBuffer indexBuffer;
        uint32_t indexCount;
    };

    struct Shader{
        std::string vertShader = ("shaders/base_vert_shader.spv");
        std::string fragShader = ("shaders/base_frag_shader.spv");
    };

    struct Texture{
        std::string texturePath;
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
        VkSampler textureSampler;

        bool isValid();
    };

    class Object{
        public:
            virtual ~Object() = default;
            virtual void draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) = 0;
            virtual void initVulkanRecourses() = 0;
            virtual void deserialize(const nlohmann::json& jsonData) = 0;
            virtual void cleanup(VkDevice device) = 0;
            std::string type;

            glm::mat4 modelMatrix{1.0f};
            glm::vec3 position;
            VkDescriptorSetLayout descriptorSetLayout;
            std::vector<VkDescriptorSet> descriptorSets;

            Mesh mesh;
            Shader shader;
            Texture texture;
    };
}

#endif