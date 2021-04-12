#pragma once

#include "CoreMinimal.h"
#include "RALResources.h"
#include <vulkan/vulkan.h>

namespace ScarletEngine
{
    class VulkanFramebuffer : public RALFramebuffer
    {
    public:
        VulkanFramebuffer(uint32_t InWidth, uint32_t InHeight, uint32_t InSamples);
        virtual ~VulkanFramebuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual void Resize(uint32_t NewWidth, uint32_t NewHeight) override;

        virtual uint64_t GetColorAttachmentID() const override;
    };

    class VulkanTexture2D : public RALTexture2D
    {
    public:
        VulkanTexture2D(const WeakPtr<TextureHandle>& InAssetHandle);

        virtual ~VulkanTexture2D();

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual uint64_t GetTextureResource() const override;
    };

    class VulkanGpuBuffer : public RALGpuBuffer
    {
    public:
        VulkanGpuBuffer(VkPhysicalDevice PhysicalDevice, VkDevice LogicalDevice, uint32_t InSize,
                        RALBufferType InType, RALBufferUsage InUsage, RALBufferPropertyFlags InProperties);
        virtual ~VulkanGpuBuffer();

        virtual void UploadData(void* DataPtr, size_t InSize) const override;
        virtual void Release() override;

        uint32_t FindMemoryType(uint32_t TypeFilter, VkMemoryPropertyFlags Properties) const;

        VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
        VkDevice LogicalDevice = VK_NULL_HANDLE;

        VkBuffer Buffer = VK_NULL_HANDLE;
        VkDeviceMemory Memory = VK_NULL_HANDLE;
    };

    class VulkanVertexArray : public RALVertexArray
    {
    public:
        VulkanVertexArray(const RALGpuBuffer* InVB, const RALGpuBuffer* InIB)
            : RALVertexArray(InVB, InIB)
        {
        }

        virtual void Bind() const override {}
        virtual void Unbind() const override {}
        virtual void Release() override {}

        // todo (vkRAL): This should be made dynamically configurable
        static VkVertexInputBindingDescription GetBindingDescription()
        {
            VkVertexInputBindingDescription BindingDescription {};
            BindingDescription.binding = 0;
            BindingDescription.stride = sizeof(Vertex);
            BindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return BindingDescription;
        }

        // todo (vkRAL): This should be made dynamically configurable
        static Array<VkVertexInputAttributeDescription> GetAttributeDescription()
        {
            Array<VkVertexInputAttributeDescription> AttributeDescriptions(1);
            AttributeDescriptions[0].binding = 0;
            AttributeDescriptions[0].location = 0;
            AttributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            AttributeDescriptions[0].offset = offsetof(Vertex, VertexPos);

            return AttributeDescriptions;
        }
    };

    class VulkanShader : public RALShader
    {
    public:
        VulkanShader(RALShaderStage Stage, const String& ShaderPath);
    private:
        friend class VulkanShaderProgram;
    };

    class VulkanShaderProgram : public RALShaderProgram
    {
    public:
        VulkanShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader, RALShader* InGeometryShader,
                            RALShader* InComputeShader);

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetUniformMat4(const glm::mat4& Mat, const char* Binding) const override;
        virtual void SetUniformVec3(const glm::vec3& Vec, const char* Binding) const override;
    };
}
