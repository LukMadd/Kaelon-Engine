#include "Instance.hpp"
#include "SwapChain.hpp"
#include "RendererGlobals.hpp"

#include <stdexcept>
#include <cstdint>
#include <set>

#include <vulkan/vulkan.hpp>



namespace renderer {
    void Instance::pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface){
        Queue queue;
        uint32_t physicalDeviceCount = 0;
        vkEnumeratePhysicalDevices(instance,&physicalDeviceCount, nullptr);

        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
        vkEnumeratePhysicalDevices(instance,&physicalDeviceCount, physicalDevices.data());

        for(const auto& currentPhysicalDevice : physicalDevices){
            QueueFamilyIndices indices = queue.FindQueueFamily(currentPhysicalDevice, surface);
            if(isDeviceSuitable(currentPhysicalDevice, surface, indices)){
                physicalDevice = currentPhysicalDevice;
                break;
            }
        }
        if(physicalDevice == VK_NULL_HANDLE){
            throw std::runtime_error("Failed to find suitable GPU");
        }
    }

    bool Instance::isDeviceSuitable(VkPhysicalDevice physDevice, VkSurfaceKHR surface, QueueFamilyIndices indices){
        VkPhysicalDeviceFeatures deviceFeatures;
        VkPhysicalDeviceProperties deviceProperties;

        vkGetPhysicalDeviceFeatures(physDevice, &deviceFeatures);
        vkGetPhysicalDeviceProperties(physDevice, &deviceProperties);

        m_extensionSupport = checkDeviceExtensionSupport(physDevice);

        bool swapChainAdequate = false;

        if(m_extensionSupport){
            SwapChainSupportDetails swapChainSupport;
            swapChainSupport = swapChainSupport.querySwapChainSupport(physDevice, surface);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(physDevice, &supportedFeatures);

        return indices.isComplete() && m_extensionSupport && swapChainAdequate && supportedFeatures.samplerAnisotropy;
    }

    bool Instance::checkDeviceExtensionSupport(VkPhysicalDevice physDevice){
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(physDevice, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtension(extensionCount);
        vkEnumerateDeviceExtensionProperties(physDevice, nullptr, &extensionCount,availableExtension.data());

        std::set<std::string> requiredExtension(m_DeviceFeatures.begin(), m_DeviceFeatures.end());

        for(const auto &extension : availableExtension){
            requiredExtension.erase(extension.extensionName);
        }

        return requiredExtension.empty();
    }

    void Instance::createLogicalDevice(VkSurfaceKHR surface, QueueFamilyIndices indices){
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority = 1.0f;
        for(uint32_t queueFamily : uniqueQueueFamilies){
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        std::vector<const char*> deviceExtensions = {
            "VK_KHR_portability_subset",
        };

        deviceExtensions.insert(deviceExtensions.end(), m_DeviceFeatures.begin(), m_DeviceFeatures.end());

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = 1;
       
        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if(enableValidationLaye){
            createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLaye.validationLaye.size());
            createInfo.ppEnabledLayerNames = m_ValidationLaye.validationLaye.data();
        } else{
            createInfo.enabledLayerCount = 0;
        }

        VkResult result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);
        if(result != VK_SUCCESS){
            throw std::runtime_error("Failed to create logicial device error code: " + std::to_string(result) + "!");
        }

        vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
    }
}
