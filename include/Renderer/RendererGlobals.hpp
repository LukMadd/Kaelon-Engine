#include <vulkan/vulkan.hpp>

#include "ModelLoader.hpp"
#include "DepthBuffer.hpp"
#include "MipMap.hpp"
#include "MultiSampling.hpp"

namespace EngineRenderer{
    inline VkQueue graphicsQueue;
    inline VkQueue presentQueue;
    inline VkDevice device;
    inline VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    inline VkCommandPool commandPool = VK_NULL_HANDLE;

    const std::string MODEL_PATH = "models/viking_room.obj";

    inline VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

    inline ModelLoader modelLoader;

    inline DepthBuffer depthBuffer;

    inline MipMap mipMap;

    inline MultiSampler multiSampler;
}