#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

#include "Texture.hpp"

using namespace EngineObject;

namespace EngineRenderer{
    struct DummyRecources{
        std::shared_ptr<Texture> texture;
        void createDummyresources();
    };

    struct UniformBufferObject{
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;

        alignas(16) glm::vec4 lightDir;
        alignas(16) glm::vec4 lightColorIntensity;
        alignas(16) glm::vec4 cameraPos;    
    };

    struct UniformBuffer{
        DummyRecources dummyresources;
        VkDescriptorSetLayout createDescriptorSetLayout(VkDescriptorSetLayout &descriptorSetLayout);
        void createUniformBuffers(int MAX_FRAMES_IN_FLIGHT, std::vector<VkBuffer> &uniformBuffers, std::vector<VkDeviceMemory> &uniformBuffersMemory, std::vector<void*> &uniformBuffersMapped);
        void createDescriptorPool(uint32_t objectCount, int MAX_FRAMES_IN_FLIGHT, VkDescriptorPool &descriptorPool);
        void createDescriptorSets(int MAX_FRAMES_IN_FLIGHT, std::vector<VkBuffer> &uniformBuffers, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool &descriptorPool, std::vector<VkDescriptorSet> &descriptorSets, const std::vector<std::shared_ptr<Texture>> &textures);
        void updateUniformBuffers(UniformBufferObject ubo, uint32_t currentImage, VkExtent2D swapChainExtent, std::vector<void*> &uniformBuffersMapped);
    };
}