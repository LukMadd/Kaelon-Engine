#include <vulkan/vulkan.hpp>
#include "ValidationLayers.hpp"
#include "Queue.hpp"

namespace renderer {
    class Instance{
        public:
            VkQueue presentQueue;

            Instance(VkInstance& instance);

            void createInstance(VkInstance &instance);

            void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

            void createLogicalDevice(VkSurfaceKHR surface, QueueFamilyIndices indices);

        private:
            bool m_extensionSupport;

            ValidationLaye m_ValidationLaye;

            const std::vector<const char*> m_DeviceFeatures = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };

            bool enableValidationLaye = false;

            bool isDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, QueueFamilyIndices indices);
            bool checkDeviceExtensionSupport(VkPhysicalDevice physDevice);
    };
}