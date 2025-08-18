#include "DepthBuffer.hpp"
#include "RendererGlobals.hpp"
#include "Utility.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace renderer{
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features){
        for(VkFormat format : candidates){
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

            if(tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features){
                return format;
            } else if(tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features){
                return format;
            }
        }
        throw std::runtime_error("Failed to find supported format!");
    }

    VkFormat DepthBuffer::findDepthFormat(){
        return findSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

    void DepthBuffer::createDepthResources(VkCommandPool commandPool, VkExtent2D swapChainExtent, VkImage &depthImage, VkDeviceMemory &depthImageMemory, VkImageView &depthImageView){
        VkFormat depthFormat = findDepthFormat();
        createImage(swapChainExtent.width, swapChainExtent.height, 1, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
        depthImageView = createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

        transitionImageLayout(commandPool, depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);
    }

    void DepthBuffer::cleanup(){
        if (depthImageView != VK_NULL_HANDLE) {
            vkDestroyImageView(device, depthImageView, nullptr);
            depthImageView = VK_NULL_HANDLE;
        }
        if (depthImage != VK_NULL_HANDLE) {
            vkDestroyImage(device, depthImage, nullptr);
            depthImage = VK_NULL_HANDLE;
        }
        if (depthImageMemory != VK_NULL_HANDLE) {
            vkFreeMemory(device, depthImageMemory, nullptr);
            depthImageMemory = VK_NULL_HANDLE;
        }
    }
}
