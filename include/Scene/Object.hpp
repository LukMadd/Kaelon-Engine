#ifndef _OBJECT_HPP
#define _OBJECT_HPP

#include "Vertex.hpp"
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
        const char* vertShader = ("shaders/base_vert_shader.spv");
        const char* fragShader = ("shaders/base_frag_shader.spv");
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
            glm::mat4 modelMatrix{1.0f};
            glm::vec3 position;
            VkDescriptorSetLayout descriptorSetLayout;
            std::vector<VkDescriptorSet> descriptorSets;

            Mesh mesh;
            Shader shader;
            Texture texture;
    };


   //Sample Class Quad 
    class Quad : public Object{
        public:
            Quad(glm::vec3 position, std::string textureImagePath = "");
            ~Quad() override;
            void draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) override;
            void initVulkanRecourses() override;

            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
    };
}

#endif