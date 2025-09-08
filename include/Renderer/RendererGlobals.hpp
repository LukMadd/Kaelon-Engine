#ifndef _RENDERER_GLOBALS_HPP
#define _RENDERER_GLOBALS_HPP

#include <vulkan/vulkan.hpp>

#include "ModelLoader.hpp"
#include "DepthBuffer.hpp"
#include "MipMap.hpp"
#include "MultiSampling.hpp"
#include "Queue.hpp"

namespace EngineRenderer{
    inline VkQueue graphicsQueue;
    inline VkQueue presentQueue;
    inline QueueFamilyIndices queueFamilyIndices;
    inline VkDevice device;
    inline VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    inline uint32_t imageCount;

    inline VkCommandPool commandPool = VK_NULL_HANDLE;
    
    inline VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

    inline ModelLoader modelLoader;

    inline DepthBuffer depthBuffer;

    inline MipMap mipMap;

    inline MultiSampler multiSampler;
}

#endif