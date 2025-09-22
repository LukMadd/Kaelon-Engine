#include <cstdint>
#include <limits>
#include "SwapChain.hpp"
#include "MultiSampling.hpp"
#include "RendererUtilities.hpp"
#include "RendererGlobals.hpp"
#include "ValidationLayers.hpp"

namespace EngineRenderer {
    SwapChainSupportDetails SwapChainSupportDetails::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface){
        SwapChainSupportDetails swapChainDetails;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device,surface, &swapChainDetails.capabilities);

        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if(formatCount != 0){
            swapChainDetails.formats.resize(formatCount);

            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, swapChainDetails.formats.data());
        }

        uint32_t presentCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentCount, nullptr);

        if(presentCount != 0){
            swapChainDetails.presentModes.resize(presentCount);

            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentCount, swapChainDetails.presentModes.data());
        } 
       
        return swapChainDetails;
    }

    void SwapChain::initSwapChain(GLFWwindow* window){
        m_window = window;
    };

    VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats){
        for(const auto& surfaceFormat : availableFormats){
            if(surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB && surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
                return surfaceFormat;
            }
        }
        return availableFormats[0];
    }

    VkPresentModeKHR SwapChain::chooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes){
        for(const auto& presentMode : availablePresentModes){
            if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR){
                return presentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabillities){
        if(surfaceCapabillities.currentExtent.width != std::numeric_limits<uint32_t>::max()){
            return surfaceCapabillities.currentExtent;
        } else{
            int width, height;

            glfwGetFramebufferSize(m_window, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, surfaceCapabillities.minImageExtent.width, surfaceCapabillities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, surfaceCapabillities.minImageExtent.height, surfaceCapabillities.maxImageExtent.height);
            
            return actualExtent;
        }
    }

    void SwapChain::createSwapChain(VkSurfaceKHR surface, QueueFamilyIndices indices){
        SwapChainSupportDetails swapChainSupport;
        swapChainSupport = swapChainSupport.querySwapChainSupport(physicalDevice, surface);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapChainPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

        imageCount = swapChainSupport.capabilities.minImageCount + 1;

        if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount){
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR swapChainCreateInfo{};
        swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapChainCreateInfo.surface = surface;

        swapChainCreateInfo.imageFormat = surfaceFormat.format;
        swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapChainCreateInfo.minImageCount = imageCount;
        swapChainCreateInfo.imageExtent = extent;
        swapChainCreateInfo.presentMode = presentMode;
        swapChainCreateInfo.imageArrayLayers = 1;
        swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        if(indices.graphicsFamily != indices.presentFamily){
            swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapChainCreateInfo.queueFamilyIndexCount = 2;
            swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else{
            swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapChainCreateInfo.queueFamilyIndexCount = 0;
            swapChainCreateInfo.pQueueFamilyIndices = nullptr;
        }

        swapChainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapChainCreateInfo.clipped = VK_TRUE;
        swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

        VkResult result = vkCreateSwapchainKHR(device, &swapChainCreateInfo, nullptr, &m_swapChain);
        if(result != VK_SUCCESS){
            throw std::runtime_error("Failed to create swap chain error code: " + std::to_string(result) + "!");
        }
        setObjectName(device, (uint64_t)m_swapChain, VK_OBJECT_TYPE_SWAPCHAIN_KHR, "Main_Swap_Chain");

        vkGetSwapchainImagesKHR(device,m_swapChain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(device, m_swapChain, &imageCount, swapChainImages.data());

        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;
    }

    void SwapChain::createFramebuffers(VkRenderPass renderPass, VkImageView depthImageView, VkImageView colorImageView){
        swapChainFramebuffers.resize(m_swapChainImageViews.size());

        for(size_t i = 0; i < m_swapChainImageViews.size(); i++){
            std::array<VkImageView, 3> attachments = {
                colorImageView, 
                depthImageView,            
                m_swapChainImageViews[i]        
            };

            VkFramebufferCreateInfo framebuffersrInfo{};
            framebuffersrInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebuffersrInfo.renderPass = renderPass;
            framebuffersrInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebuffersrInfo.pAttachments = attachments.data();
            framebuffersrInfo.width = swapChainExtent.width;
            framebuffersrInfo.height = swapChainExtent.height;
            framebuffersrInfo.layers = 1;

            VkResult result = vkCreateFramebuffer(device, &framebuffersrInfo, nullptr, &swapChainFramebuffers[i]);
            if(result != VK_SUCCESS){
                throw std::runtime_error("Failed to create framebuffersr at index " + std::to_string(i) + " error code: " + std::to_string(result) + "!");
            }
            setObjectName(device, (uint64_t)swapChainFramebuffers[i], VK_OBJECT_TYPE_FRAMEBUFFER, "Frame_Buffer");
        }
    }

    VkSwapchainKHR SwapChain::getSwapChain(){
        return m_swapChain;
    }

    void SwapChain::createImageViews(){
        m_swapChainImageViews.resize(swapChainImages.size());

        for(size_t i = 0; i < swapChainImages.size(); i++){
            m_swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
        }
    }

    void SwapChain::recreateSwapChain(VkSurfaceKHR surface, QueueFamilyIndices indices, VkRenderPass renderPass, DepthBuffer &depthBuffer, MultiSampler &multiSampler, GLFWwindow* window){
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        while(width == 0 || height == 0){
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(device);

        cleanupSwapChain(depthBuffer, multiSampler);

        createSwapChain(surface, indices);
        createImageViews();
        multiSampler.createColorResources(swapChainImageFormat, swapChainExtent);
        depthBuffer.createDepthResources(swapChainExtent, depthBuffer.depthImage, depthBuffer.depthImageMemory, depthBuffer.depthImageView);
        createFramebuffers(renderPass, depthBuffer.depthImageView, multiSampler.colorImageView);
    }

    void SwapChain::cleanupSwapChain(DepthBuffer &depthBuffer, MultiSampler &multiSampler){
        for(auto framebuffersr : swapChainFramebuffers){
            vkDestroyFramebuffer(device, framebuffersr, nullptr);
        }

        for(auto imageView : m_swapChainImageViews){
            if (imageView != VK_NULL_HANDLE) vkDestroyImageView(device, imageView, nullptr);
        }

        vkDestroySwapchainKHR(device, m_swapChain, nullptr);

        depthBuffer.cleanup();

        multiSampler.cleanup();
    }
}