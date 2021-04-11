#include "VulkanResources.h"

#define CHECK_RESULT(Res) check(Res == VK_SUCCESS)

namespace ScarletEngine
{
    VulkanGpuBuffer::VulkanGpuBuffer(VkPhysicalDevice InPhysicalDevice, VkDevice InLogicalDevice, uint32_t InSize,
                                     RALBufferUsage InUsage)
        : RALGpuBuffer(InSize, InUsage)
          , PhysicalDevice(InPhysicalDevice)
          , LogicalDevice(InLogicalDevice)
    {
        VkBufferCreateInfo BufferCreateInfo {};
        BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        BufferCreateInfo.size = Size;
        
        switch (Usage)
        {
        case RALBufferUsage::STATIC_DRAW:
        case RALBufferUsage::DYNAMIC_DRAW:
            BufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            // todo (vkRAL): this is not exactly correct but will suffice for now
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
        void* Mapped = nullptr;
        vkMapMemory(LogicalDevice, Memory, 0, InSize, 0, &Mapped);
        check(Mapped != nullptr);
        memcpy(Mapped, DataPtr, InSize);
    }
}
