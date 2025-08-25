#ifndef _INSTANCE_HPP
#define _INSTANCE_HPP

#include <vulkan/vulkan.hpp>
#include "ValidationLayers.hpp"
#include "Queue.hpp"

namespace EngineRenderer {
    class Instance{
        public:
            VkQueue presentQueue;

            Instance(VkInstance& instance);

            void createInstance(VkInstance &instance);

            void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

            VkDevice createLogicalDevice(VkSurfaceKHR surface, QueueFamilyIndices indices);

        private:
            bool m_extensionSupport;

            ValidationLayers m_ValidationLayers;

            const std::vector<const char*> m_DeviceFeatures = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };

            bool enableValidationLayers = false;

            bool isDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, QueueFamilyIndices indices);
            bool checkDeviceExtensionSupport(VkPhysicalDevice physDevice);
    };
}

#endif