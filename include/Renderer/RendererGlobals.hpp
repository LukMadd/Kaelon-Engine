#ifndef _RENDERER_GLOBALS_HPP
#define _RENDERER_GLOBALS_HPP

#include <cstdint>
#include <vulkan/vulkan.hpp>

#include "ModelLoader.hpp"
#include "DepthBuffer.hpp"
#include "MipMap.hpp"
#include "MultiSampling.hpp"
#include "Queue.hpp"

constexpr uint32_t MAX_TEXTURES = 16;
constexpr uint32_t MAX_OBJECTS = 1750;

constexpr uint32_t WINDOW_INIT_WIDTH = 1000;
constexpr uint32_t WINDOW_INIT_HEIGHT = 800;

namespace EngineRenderer{
    inline VkQueue graphicsQueue;
    inline VkQueue presentQueue;
    inline QueueFamilyIndices queueFamilyIndices;
    inline VkDevice device;
    inline VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    inline int drawCallCount = 0;
    inline int drawCallCountLastFrame = 0;

    inline bool wireFrameModeEnabled = false;

    inline uint32_t imageCount;

    inline VkCommandPool commandPool = VK_NULL_HANDLE;
    
    inline VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

    inline ModelLoader modelLoader;

    inline DepthBuffer depthBuffer;

    inline MipMap mipMap;

    inline MultiSampler multiSampler;
}

#endif