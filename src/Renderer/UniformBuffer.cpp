#include "UniformBuffer.hpp"
#include "RendererUtilities.hpp"
#include "RendererGlobals.hpp"
#include "ValidationLayers.hpp"
#include <array>
#include <cstdint>
#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace EngineRenderer{
    void DummyRecources::createDummyResources(){
        texture = std::make_shared<Texture>();
        
        uint32_t whitePixel = 0xFFFFFFFF;
        VkDeviceSize imageSize = sizeof(whitePixel);

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingMemory;

        createBuffer("Dummy_Buffer", imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingMemory);
        void* data;
        vkMapMemory(device, stagingMemory, 0, imageSize, 0, &data);
        memcpy(data, &whitePixel, static_cast<size_t>(imageSize));
        vkUnmapMemory(device, stagingMemory);

        createImage(1, 1, 1, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, texture->textureImage, texture->textureImageMemory);
        transitionImageLayout(commandPool, texture->textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1);

        texture->textureImageView = createImageView(texture->textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, 1);

        copyBufferToImage(commandPool, stagingBuffer, texture->textureImage, 1, 1);

        transitionImageLayout(commandPool, texture->textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1);

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = 1.0f;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

        if(vkCreateSampler(device, &samplerInfo, nullptr, &texture->textureSampler) != VK_SUCCESS){
            throw std::runtime_error("Failed to create dummy sampler!");
        }
        setObjectName(device, (uint64_t)texture->textureSampler, VK_OBJECT_TYPE_SAMPLER, "Default_Resource_Sampler");

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingMemory, nullptr);
    }

     VkDescriptorSetLayout UniformBuffer::createDescriptorSetLayout(VkDescriptorSetLayout &descriptorSetLayout){
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorSetLayoutBinding objectUboBinding{};
        objectUboBinding.binding = 2;
        objectUboBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        objectUboBinding.descriptorCount = 1;
        objectUboBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 3> bindings = {uboLayoutBinding, samplerLayoutBinding, objectUboBinding};
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        VkResult result = vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout);
        if(result != VK_SUCCESS){
            throw std::runtime_error("Failed to create descriptor set layout error code: " + std::to_string(result) + "!");
        }

        return descriptorSetLayout;
    }

    void DummyRecources::cleanupDummyRecourses(){
        vkDestroyImageView(device, texture->textureImageView, nullptr);
        vkDestroyImage(device, texture->textureImage, nullptr);
        vkFreeMemory(device, texture->textureImageMemory, nullptr);
        vkDestroySampler(device, texture->textureSampler, nullptr);
    }



    void UniformBuffer::createUniformBuffers(int MAX_FRAMES_IN_FLIGHT, VkDeviceSize bufferSize, std::vector<VkBuffer> &uniformBuffers, std::vector<VkDeviceMemory> &uniformBuffersMemory, std::vector<void*> &uniformBuffersMapped){
        uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
            createBuffer("Uniform_Buffer_Index_" + std::to_string(i) + "_size_" + std::to_string(bufferSize), bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

            vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
        }
    }

    void UniformBuffer::createDescriptorPool(uint32_t objectCount, int MAX_FRAMES_IN_FLIGHT, VkDescriptorPool &descriptorPool){
            std::array<VkDescriptorPoolSize, 2> poolSizes{};
            poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * objectCount * 2);
            poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * objectCount); 

            VkDescriptorPoolCreateInfo poolInfo{};
            poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
            poolInfo.pPoolSizes = poolSizes.data();
            poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * objectCount);

            VkResult result = vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool);
            if(result != VK_SUCCESS){
                throw std::runtime_error("Failed to create descriptor pool erorr code: " + std::to_string(result) + "!");
            }    
            setObjectName(device, (uint64_t)descriptorPool, VK_OBJECT_TYPE_DESCRIPTOR_POOL, "Main_Descriptor_Pool");
        }

    void UniformBuffer::createDescriptorSets(int MAX_FRAMES_IN_FLIGHT, std::vector<VkBuffer> &uniformBuffers, std::vector<VkBuffer> &objectUniformBuffers, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool &descriptorPool, std::vector<VkDescriptorSet> &descriptorSets, const std::vector<std::shared_ptr<Texture>> &textures){
        descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
        if(!dummyResources.texture){
            dummyResources.createDummyResources();
        }

        for (size_t frame = 0; frame < MAX_FRAMES_IN_FLIGHT; ++frame) {
            if(descriptorSets[frame] == VK_NULL_HANDLE){
                continue;
            }
        
        for(size_t frame = 0; frame < MAX_FRAMES_IN_FLIGHT; frame++){
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffers[frame];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);
            VkDescriptorBufferInfo objectBufferInfo{};
            objectBufferInfo.buffer = objectUniformBuffers[frame]; // provide this vector to the function
            objectBufferInfo.offset = 0;
            objectBufferInfo.range  = sizeof(ObjectUBO);

            VkDescriptorImageInfo imageInfo{};
            if(!textures.empty() && textures[0]->textureImageView != VK_NULL_HANDLE){
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = textures[0]->textureImageView;
                imageInfo.sampler = textures[0]->textureSampler;
            } else{
                assert(dummyResources.texture->textureImageView != VK_NULL_HANDLE);
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = dummyResources.texture->textureImageView;
                imageInfo.sampler = dummyResources.texture->textureSampler;
            }

            std::vector<VkWriteDescriptorSet> descriptorWrites;

            VkWriteDescriptorSet uboWrite{};
            uboWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            uboWrite.dstSet = descriptorSets[frame];
            uboWrite.dstBinding = 0;
            uboWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            uboWrite.descriptorCount = 1;
            uboWrite.pBufferInfo = &bufferInfo;
            descriptorWrites.push_back(uboWrite);

            VkWriteDescriptorSet textureWrite{};
            textureWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            textureWrite.dstSet = descriptorSets[frame];
            textureWrite.dstBinding = 1;
            textureWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            textureWrite.descriptorCount = 1;
            textureWrite.pImageInfo = &imageInfo;
            descriptorWrites.push_back(textureWrite);

            VkWriteDescriptorSet objectUboWrite{};
            objectUboWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            objectUboWrite.dstSet = descriptorSets[frame];
            objectUboWrite.dstBinding = 2;
            objectUboWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            objectUboWrite.descriptorCount = 1;
            objectUboWrite.pBufferInfo = &objectBufferInfo;
            descriptorWrites.push_back(objectUboWrite);

            vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }
    }
    }    

    void UniformBuffer::updateUniformBuffers(UniformBufferObject ubo, float fov, uint32_t currentImage, VkExtent2D swapChainExtent, std::vector<void*> &uniformBuffersMapped){
        ubo.proj = glm::perspective(glm::radians(fov), swapChainExtent.width / (float) swapChainExtent.height, 0.1f, 100.0f);

        ubo.proj[1][1]*=-1;

        memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
    }

    void UniformBuffer::updateObjectUniformBuffers(ObjectUBO objectUbo, std::vector<void*> &uniformBuffersMapped, uint32_t currentImage){
        memcpy(uniformBuffersMapped[currentImage], &objectUbo, sizeof(objectUbo));
    }
}