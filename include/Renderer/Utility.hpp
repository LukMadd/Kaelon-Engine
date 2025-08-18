#include <vulkan/vulkan.hpp>
namespace renderer{
    void createBuffer(VkDeviceSize &size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);
    VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool);
    void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkCommandPool commandPool);;
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool &commandPool, VkQueue &graphicsQueue);
    void copyBufferToImage(VkCommandPool commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    bool hasStencilComponent(VkFormat format);
    void transitionImageLayout(VkCommandPool commandPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
    void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
}