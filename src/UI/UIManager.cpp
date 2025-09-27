#include "UIManager.hpp"

#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_glfw.h"

#include "RendererGlobals.hpp"
#include "RendererUtilities.hpp"
#include <cassert>
#include <iostream>

namespace EngineUI{
    //Initializes ImGui
    void UIManager::initImGui(UIInfo &uiInfoRef){
        uiInfo = uiInfoRef;
        if(!uiInfo.sceneManager){
            throw std::runtime_error("INVALID SCENE MANAGER");
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui::GetStyle().ScaleAllSizes(io.DisplayFramebufferScale.x);
        io.Fonts->AddFontFromFileTTF("../fonts/PixeloidMono.ttf", io.DisplayFramebufferScale.x * scale);
        baseStyle = ImGui::GetStyle();

        ImGui_ImplGlfw_InitForVulkan(uiInfo.window, true);
        ImGui_ImplVulkan_PipelineInfo pipeline_info{};
        pipeline_info.RenderPass = uiInfo.renderPass;
        pipeline_info.MSAASamples = msaaSamples;

        ImGui_ImplVulkan_InitInfo init_info{};
        init_info.Instance = uiInfo.instance;
        init_info.PhysicalDevice = physicalDevice;
        init_info.Device = device;
        init_info.QueueFamily = queueFamilyIndices.graphicsFamily.value();
        init_info.Queue = graphicsQueue;
        init_info.PipelineCache = uiInfo.pipelineCache;
        init_info.PipelineInfoMain = pipeline_info;
        init_info.DescriptorPool = uiInfo.imGuiDescriptorPool;
        init_info.MinImageCount = imageCount;
        init_info.ImageCount = imageCount;
        init_info.CheckVkResultFn = [](VkResult err) { 
            if (err != VK_SUCCESS) std::cerr << "ImGui-Vulkan Error: " << err << "\n"; 
        };
        ImGui_ImplVulkan_Init(&init_info);
    }

    //Creates a descriptor pool specifically for imgui
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

    void UIManager::beginFrame(float fps){
        int frameBufferWidth, frameBufferHeight;
        glfwGetFramebufferSize(uiInfo.window, &frameBufferWidth, &frameBufferHeight);

        int windowWidth, windowHeight;
        glfwGetWindowSize(uiInfo.window, &windowWidth, &windowHeight);

        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)frameBufferWidth, (float)frameBufferHeight);
        io.DisplayFramebufferScale = ImVec2(
            windowWidth > 0 ? (float)frameBufferWidth / windowWidth : 1.0f,
            windowHeight > 0 ? (float)frameBufferHeight / windowHeight : 1.0f
        );
        

        float contentScaleX, contentScaleY;
        glfwGetWindowContentScale(uiInfo.window, &contentScaleX, &contentScaleY);
        static float previous_scale = contentScaleX;

        if(contentScaleX != previous_scale){
            if(contentScaleX < previous_scale){
                io.FontGlobalScale = 1 + (contentScaleX / (contentScaleX / 1.1)); //I don't know but it is close enough
            } else{
                io.FontGlobalScale = 1;
            }
            previous_scale = contentScaleX;

            ImGuiStyle& style = ImGui::GetStyle();
            style = baseStyle; //Prevents exponential scaling(ScaleAllSizes multiples _MainScale by input)
            style.ScaleAllSizes(io.FontGlobalScale);
        }

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //Create a dock space
        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar |
                                ImGuiWindowFlags_NoMove |
                                ImGuiWindowFlags_NoBringToFrontOnFocus |
                                ImGuiWindowFlags_NoNavFocus |
                                ImGuiWindowFlags_NoBackground |
                                ImGuiWindowFlags_MenuBar;

        ImGui::Begin("Dock_Space_Window", nullptr, window_flags);

        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
        
        ImGuiID dockspaceID = ImGui::GetID("Dock_Space");
        ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspace_flags);
        
        engineUI.drawMainLayout(uiInfo.sceneManager);
        engineUI.drawSceneHierarchy(uiInfo.sceneManager->getCurrentScene());
        engineUI.drawObjectInspector(uiInfo.sceneManager->getCurrentScene());
        engineUI.drawCameraInspector();
        engineUI.drawRecourses(uiInfo.recourseManager);
        engineUI.drawRenderStats(uiInfo.sceneManager->getCurrentScene() ,fps);
        engineUI.drawScenes(uiInfo.sceneManager);
        engineUI.drawSceneInspector(uiInfo.sceneManager);

        ImGui::End();
        ImGui::PopStyleVar(2);
    }

    void UIManager::renderUI(float fps){
        beginFrame(fps);

        ImGui::Render();
        //ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData()); happens in recordCommandBuffers() in command.cpp
    }

    void UIManager::shutDownImGui(VkDescriptorPool &imGuiDescriptorPool){
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        vkDestroyDescriptorPool(device, imGuiDescriptorPool, nullptr);
    }
}