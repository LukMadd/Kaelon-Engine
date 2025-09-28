#include "Renderer/MultiSampling.hpp"
#include "Renderer/RendererGlobals.hpp"
#include "Renderer/RendererUtilities.hpp"

namespace EngineRenderer{
    void MultiSampler::createColorResources(VkFormat swapChainImageFormat, VkExtent2D swapChainExtent){
        VkFormat colorFormat = swapChainImageFormat;

        createImage(swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);

        colorImageView = createImageView(colorImage, colorFormat,VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void MultiSampler::cleanup(){
        vkDestroyImageView(device, colorImageView, nullptr);
        vkDestroyImage(device, colorImage, nullptr);
        vkFreeMemory(device, colorImageMemory, nullptr);
    }
}