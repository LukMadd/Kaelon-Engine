#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

#include "Misc/Texture.hpp"

;

namespace EngineRenderer{
    struct DummyRecources{
        std::shared_ptr<Texture> texture;
        void createDummyResources();

        void cleanupDummyRecourses();
    };

    struct UniformBufferObject{
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;

        alignas(16) glm::vec4 lightDir;
        alignas(16) glm::vec4 lightColorIntensity;
        alignas(16) glm::vec4 cameraPos;    
    };

    struct ObjectUBO{
        alignas(16) glm::vec4 baseColor;

        alignas(4) int hasTexture;

        alignas(4) int textureIndex;
    };


    struct UniformBuffer{
        DummyRecources dummyResources;
        void createDescriptorSetLayout(VkDescriptorSetLayout &descriptorSetLayout);
        void createUniformBuffers(int MAX_FRAMES_IN_FLIGHT, VkDeviceSize bufferSize, std::vector<VkBuffer> &uniformBuffers, std::vector<VkDeviceMemory> &uniformBuffersMemory, std::vector<void*> &uniformBuffersMapped);
        void createDescriptorPool(int MAX_FRAMES_IN_FLIGHT, VkDescriptorPool &descriptorPool);
        void createDescriptorSets(int MAX_FRAMES_IN_FLIGHT, std::vector<VkBuffer> &uniformBuffers, std::vector<VkBuffer> &objectUniformBuffers, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool &descriptorPool, std::vector<VkDescriptorSet> &descriptorSets, const std::vector<std::shared_ptr<Texture>> &textures);
        void updateUniformBuffers(UniformBufferObject ubo, float fov, uint32_t currentImage, VkExtent2D swapChainExtent, std::vector<void*> &uniformBuffersMapped);
        void updateObjectUniformBuffers(ObjectUBO objectUbo, std::vector<void*> &uniformBuffersMapped, uint32_t currentImage);

        void cleanup() {dummyResources.cleanupDummyRecourses();}
    };
}