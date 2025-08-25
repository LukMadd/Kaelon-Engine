#include <vector>
#include <vulkan/vulkan.hpp>

void setObjectName(VkDevice device, uint64_t objectHandle, VkObjectType objectType, const std::string& name);
namespace EngineRenderer {
    
    inline VkDebugUtilsMessengerEXT debugMessenger;
    class ValidationLayers{
        public:
            bool CheckValidationSupport();

            static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
            void createDebugMessenger(VkInstance instance);
           
            const std::vector<const char*> validationLayers{
                "VK_LAYER_KHRONOS_validation"
            };
    };
}