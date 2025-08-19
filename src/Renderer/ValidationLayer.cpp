#include "ValidationLayers.hpp"
#include <cstring>
#include <vulkan/vulkan.hpp>

namespace EngineRenderer {
    bool ValidationLayers::CheckValidationSupport(){
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLaye(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLaye.data());

        for(const char* layerName : validationLaye){
            bool layerFound = false;

             for(const auto& layerProperty : availableLaye){
                if(strcmp(layerName, layerProperty.layerName) == 0){
                    layerFound = true;
                    break;
                }
             }
             
             if(!layerFound){
                return false;
             }
        }
    return true;
    }
}