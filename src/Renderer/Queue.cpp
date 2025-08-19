#include "Queue.hpp"
#include <cstdint>

namespace EngineRenderer {
    QueueFamilyIndices Queue::FindQueueFamily(VkPhysicalDevice device, VkSurfaceKHR surface){
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        VkBool32 presentSupport;

        for(int i = 0; i < queueFamilies.size(); i++){
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i,surface, &presentSupport);

            if(queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
                indices.graphicsFamily = i;
            }

            if(presentSupport){
                indices.presentFamily = i;
            }

            if(indices.isComplete()){
                break;
            }
        }

        return indices;
    }

    bool QueueFamilyIndices::isComplete(){
        return graphicsFamily.has_value();
    }
}