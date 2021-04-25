#include "VulkanResources.h"

#define CHECK_RESULT(Res) check(Res == VK_SUCCESS)

namespace ScarletEngine
{

    VulkanFramebuffer::VulkanFramebuffer(VkDevice LogicalDevice, uint32_t InWidth, uint32_t InHeight, 
        uint32_t InSamples, VkImage ColorImage, VkImageView ColorImageView, VkDeviceMemory ColorImageMemory,
        VkSampler ColorImageSampler, VkFramebuffer Framebuffer) 
        : RALFramebuffer(InWidth, InHeight, InSamples)
        , device(LogicalDevice), colorImage(ColorImage), colorImageView(ColorImageView)
        , colorImageMemory(ColorImageMemory), colorImageSampler(ColorImageSampler), framebuffer(Framebuffer)
    {
    }

    VulkanFramebuffer::~VulkanFramebuffer() 
    {
        ZoneScoped
        vkDestroyImage(device, colorImage, nullptr);
        vkDestroyImageView(device, colorImageView, nullptr);
        vkFreeMemory(device, colorImageMemory, nullptr);
        vkDestroySampler(device, colorImageSampler, nullptr);
        vkDestroyFramebuffer(device, framebuffer, nullptr);

    }

    // TODO: implement these 
    void VulkanFramebuffer::Bind() const {}
    void VulkanFramebuffer::Unbind() const {}
    void VulkanFramebuffer::Resize(uint32_t NewWidth, uint32_t NewHeight) {}
    uint64_t VulkanFramebuffer::GetColorAttachmentID() const { return 1;}

    VulkanGpuBuffer::VulkanGpuBuffer(VkPhysicalDevice InPhysicalDevice, VkDevice InLogicalDevice, uint32_t InSize,
        RALBufferType InType, RALBufferUsage InUsage, RALBufferPropertyFlags InProperties)
        : RALGpuBuffer(InSize, InType, InUsage, InProperties)
          , PhysicalDevice(InPhysicalDevice)
          , LogicalDevice(InLogicalDevice)
    {
        VkBufferCreateInfo BufferCreateInfo {};
        BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        BufferCreateInfo.size = Size;
        
        switch (Type)
        {
        case RALBufferType::VERTEX_BUFFER:
            BufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            break;
        case RALBufferType::INDEX_BUFFER:
            BufferCreateInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            break;
        default:
            break;
        }
        
        BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        CHECK_RESULT(vkCreateBuffer(LogicalDevice, &BufferCreateInfo, nullptr, &Buffer));

        VkMemoryRequirements MemoryRequirements {};
        vkGetBufferMemoryRequirements(LogicalDevice, Buffer, &MemoryRequirements);

        VkMemoryAllocateInfo AllocateInfo {};
        AllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        AllocateInfo.allocationSize = MemoryRequirements.size;
        AllocateInfo.memoryTypeIndex = FindMemoryType(MemoryRequirements.memoryTypeBits,
                                                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                                      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        CHECK_RESULT(vkAllocateMemory(LogicalDevice, &AllocateInfo, nullptr, &Memory));

        vkBindBufferMemory(LogicalDevice, Buffer, Memory, 0);
    } 

    VulkanGpuBuffer::~VulkanGpuBuffer()
    {
        Release();
    }

    void VulkanGpuBuffer::Release()
    {
        vkDestroyBuffer(LogicalDevice, Buffer, nullptr);
        vkFreeMemory(LogicalDevice, Memory, nullptr);
    }

    uint32_t VulkanGpuBuffer::FindMemoryType(uint32_t TypeFilter, VkMemoryPropertyFlags Properties) const
    {
        VkPhysicalDeviceMemoryProperties MemoryProperties {};
        vkGetPhysicalDeviceMemoryProperties(PhysicalDevice, &MemoryProperties);

        for (uint32_t i = 0; i < MemoryProperties.memoryTypeCount; ++i)
        {
            if ((TypeFilter & (1 << i)) && (MemoryProperties.memoryTypes[i].propertyFlags & Properties) == Properties)
            {
                return i;
            }
        }

        return UINT32_MAX;
    }

    void VulkanGpuBuffer::UploadData(void* DataPtr, size_t InSize) const
    {
        if((Properties & RALBufferPropertyFlagBits::HOST_VISIBLE_BIT) &&
            (Properties & RALBufferPropertyFlagBits::HOST_COHERENT_BIT)) {
            void* Mapped = nullptr;
            vkMapMemory(LogicalDevice, Memory, 0, InSize, 0, &Mapped);
            check(Mapped != nullptr);
            memcpy(Mapped, DataPtr, InSize);
        }
    }
}
