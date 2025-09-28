#include "Renderer/ValidationLayers.hpp"
#include <cstring>
#include <iostream>
#include <vulkan/vulkan.hpp>

void setObjectName(VkDevice device, uint64_t objectHandle, VkObjectType objectType, const std::string& name){
    VkDebugUtilsObjectNameInfoEXT nameInfo{};
    nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
    nameInfo.objectType = objectType;
    nameInfo.objectHandle = objectHandle;
    nameInfo.pObjectName = name.c_str();

    auto func = (PFN_vkSetDebugUtilsObjectNameEXT) vkGetDeviceProcAddr(device, "vkSetDebugUtilsObjectNameEXT");
    if(func != nullptr){
        func(device, &nameInfo);
    }
}

namespace EngineRenderer {
    bool ValidationLayers::CheckValidationSupport(){
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLaye(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLaye.data());

        for(const char* layerName : validationLayers){
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

    VKAPI_ATTR VkBool32 VKAPI_CALL ValidationLayers::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData){
        std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }

    void ValidationLayers::createDebugMessenger(VkInstance instance){
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = 
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = 
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
        createInfo.pUserData = nullptr;


        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, &createInfo, nullptr, &debugMessenger);
        }
    }
}