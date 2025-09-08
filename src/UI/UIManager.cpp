#include "UIManager.hpp"
#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_glfw.h"

#include "RendererGlobals.hpp"
#include "RendererUtilities.hpp"
#include <iostream>

namespace EngineUI{
    void UIManager::initImGui(
        GLFWwindow* window, 
        VkInstance instance, 
        VkPipelineCache pipelineCache, 
        VkDescriptorPool imGuiDescriptorPool, 
        VkRenderPass renderPass){

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo init_info{};
        init_info.Instance = instance;
        init_info.PhysicalDevice = physicalDevice;
        init_info.Device = device;
        init_info.QueueFamily = queueFamilyIndices.graphicsFamily.value();
        init_info.Queue = graphicsQueue;
        init_info.PipelineCache = pipelineCache;
        init_info.RenderPass = renderPass;
        init_info.MSAASamples = msaaSamples;
        init_info.DescriptorPool = imGuiDescriptorPool;
        init_info.MinImageCount = imageCount;
        init_info.ImageCount = imageCount;
        init_info.CheckVkResultFn = [](VkResult err) { 
            if (err != VK_SUCCESS) std::cerr << "ImGui-Vulkan Error: " << err << "\n"; 
        };
        ImGui_ImplVulkan_Init(&init_info);
    }

    VkDescriptorPool UIManager::createImGuiDescriptorPool(){
        VkDescriptorPoolSize poolSizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1000 * (uint32_t)std::size(poolSizes);
        poolInfo.poolSizeCount = (uint32_t)std::size(poolSizes);
        poolInfo.pPoolSizes = poolSizes;
        
        VkDescriptorPool imguiDescriptorPool = VK_NULL_HANDLE;
        VkResult result = vkCreateDescriptorPool(device, &poolInfo, nullptr, &imguiDescriptorPool);
        if(result != VK_SUCCESS) throw std::runtime_error("Failed to create ImGui descriptor pool error code: " + std::to_string(result) + "!");
        return imguiDescriptorPool;
    }

    void UIManager::beginFrame(GLFWwindow* window, float fps){
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)width, (float)height);
        io.FontGlobalScale = 1.5f; 

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(75, 75), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::Begin("FPS");
        ImGui::Text("%.1f", fps);
        ImGui::End();

        ImGui::Render();
    }

    void UIManager::shutDownImGui(VkDescriptorPool &imGuiDescriptorPool){
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        vkDestroyDescriptorPool(device, imGuiDescriptorPool, nullptr);
    }
}