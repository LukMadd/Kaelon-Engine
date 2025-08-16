#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

namespace renderer{
    struct UniformBufferObject{
        alignas(16)glm::mat4 model;
        alignas(16)glm::mat4 view;
        alignas(16)glm::mat4 proj;
    };

    struct UniformBuffer{
        void createDescriptorSetLayout(VkDescriptorSetLayout &descriptorSetLayout);
        void createUniformBuffers(int MAX_FRAMES_IN_FLIGHT, std::vector<VkBuffer> &uniformBuffers, std::vector<VkDeviceMemory> &uniformBuffersMemory, std::vector<void*> &uniformBuffersMapped, VkPhysicalDevice physicalDevice, VkDevice device);
        void createDescriptorPool(int MAX_FRAMES_IN_FLIGHT, VkDescriptorPool &descriptorPool);
        void createDescriptorSets(int MAX_FRAMES_IN_FLIGHT, std::vector<VkBuffer> &uniformBuffers, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool &descriptorPool, std::vector<VkDescriptorSet> &descriptorSets, VkImageView textureImageView, VkSampler textureSampler);
        void updateUniformBuffers(uint32_t currentImage, VkExtent2D swapChainExtent, std::vector<void*> &uniformBuffersMapped);
    };
}