#pragma once

#include "Core.h"
#include "RAL.h"

// #todo: Currently leaking this include, need to find a way to move this
#include <vulkan/vulkan.h>

namespace ScarletEngine
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> GraphicsFamily;
        std::optional<uint32_t> PresentFamily;

        bool IsComplete() const
        {
            return GraphicsFamily.has_value() && PresentFamily.has_value();
        }
    };

    class ImGuiVulkanObjects {
    public:
        ImGuiVulkanObjects();
        void Initialize(VkDevice device, VkFormat swapChainImageFormat, 
            uint32_t width, uint32_t height, QueueFamilyIndices queueFamilyIndices, const std::vector<VkImageView>& swapChainImageViews);
        void CleanUp(VkDevice device);
    private:
        void CreateRenderPass(VkDevice device, VkFormat swapChainFormat);
        void CreateCommandPool(VkDevice device, QueueFamilyIndices queueFamilyIndices);
        void CreateCommandBuffers();
        void CreateFramebuffers(VkDevice device, uint32_t width, uint32_t height, const std::vector<VkImageView>& attachments);

        VkRenderPass RenderPass;
        VkCommandPool CommandPool;
        VkDescriptorPool DescriptorPool;
        std::vector<VkCommandBuffer> CommandBuffers;
        std::vector<VkFramebuffer> Framebuffers;
    };

    class VulkanRAL : public RAL
    {
    public:
        VulkanRAL();

        virtual ~VulkanRAL()
        {
        }

        // Begin RAL Interface
        virtual void Initialize() override;
        virtual void Terminate() override;
        virtual void PreFrame() override;
        virtual void Submit() override;

        virtual const char* GetBackendName() const override
        {
            return "Vulkan";
        }

        virtual GPUInfo GetGPUInfo() const override;

        virtual void SetClearColorCmd(const glm::vec4& ClearColor) override;
        virtual void ClearCmd(bool bColor, bool bDepth, bool bStencil) override;
        virtual void DrawVertexArrayCmd(const RALVertexArray* VA) override;

        virtual RALFramebuffer* CreateFramebuffer(uint32_t Width, uint32_t Height, uint32_t Samples) override;
        virtual RALTexture2D* CreateTexture2D(const WeakPtr<TextureHandle>& AssetHandle) override;
        virtual RALGpuBuffer* CreateBuffer(uint32_t Size, RALBufferType Type, RALBufferUsage Usage, RALBufferPropertyFlags Properties) override;
        virtual RALVertexArray* CreateVertexArray(const RALGpuBuffer* VB, const RALGpuBuffer* IB) override;
        virtual RALShader* CreateShader(RALShaderStage Stage, const String& ShaderPath) override;
        virtual RALShaderProgram* CreateShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader,
                                                      RALShader* InGeometryShader, RALShader* InComputeShader) override;
    private:
        virtual RALCommandList* CreateCommandList() const override;
        // End RAL Interface

        bool CheckForLayerSupport(String* OutError = nullptr) const;
        void CreateInstance();
        void SetupDebugMessenger();

        void CreateImGuiVulkanObjects();

        struct SwapchainSupportDetails
        {
            VkSurfaceCapabilitiesKHR Capabilities;
            Array<VkSurfaceFormatKHR> Formats;
            Array<VkPresentModeKHR> PresentModes;
        };

        void CreateSurface();

        QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice Device) const;
        SwapchainSupportDetails QuerySwapchainSupport(const VkPhysicalDevice Device) const;
        bool CheckForDeviceExtensionSupport(const VkPhysicalDevice Device, String* OutError = nullptr) const;
        bool IsPhysicalDeviceSuitable(const VkPhysicalDevice Device) const;
        void PickPhysicalDevice();
        void CreateLogicalDevice();

        static VkSurfaceFormatKHR ChooseSwapchainSurfaceFormat(const Array<VkSurfaceFormatKHR>& AvailableFormats);
        static VkPresentModeKHR ChooseSwapchainPresentMode(const Array<VkPresentModeKHR>& AvailablePresentModes);
        static VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& Capabilities);
        void CreateSwapchain();
        void CreateSwapchainImageViews();

        static Array<char> ReadShaderFile(const String& Filename);
        VkShaderModule CreateShaderModule(const Array<char>& Code) const;
        void CreateRenderPass();
        void CreateGraphicsPipeline();

        void CreateFramebuffers();

        void CreateCommandPool();
        void CreateCommandBuffers();

        // Generic util methods for image related objects, probably best
        // to move these somewhere else to reduce size of impl file
        VkFormat GetSupportedDepthFormat(const std::vector<VkFormat>& Candidates, VkImageTiling tiling, 
            VkFormatFeatureFlags Features);
        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void CreateImage(uint32_t Width, uint32_t Height, VkFormat format, VkImageTiling Tiling, 
            VkImageUsageFlags Usage, VkMemoryPropertyFlags Properties, VkImage& Image, VkDeviceMemory& ImageMemory);
        VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        VkSampler CreateSampler(VkFilter magFilter, VkFilter minFilter, 
            VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode);

        void BeginRenderPassCommandBuff(VkCommandBuffer& CmdBuff); // @todo: remove when pipeline is properly abstracted
        void EndRenderPassCommandBuff(VkCommandBuffer& CmdBuff); // @todo: remove when pipeline is properly abstracted

        void CreateSyncObjects();

        void CleanupSwapchain();
        void RebuildSwapchain();
    private:
        VkInstance Instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT DebugMessenger = VK_NULL_HANDLE;

        VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
        VkDevice LogicalDevice = VK_NULL_HANDLE;
        VkQueue GraphicsQueue = VK_NULL_HANDLE;
        VkQueue PresentQueue = VK_NULL_HANDLE;

        VkSurfaceKHR Surface = VK_NULL_HANDLE;
        VkSwapchainKHR Swapchain = VK_NULL_HANDLE;
        VkFormat SwapchainImageFormat;
        VkExtent2D SwapchainImageExtent;
        Array<VkImage> SwapchainImages;
        Array<VkImageView> SwapchainImageViews;
        Array<VkFramebuffer> SwapchainFramebuffers;

        VkRenderPass RenderPass = VK_NULL_HANDLE;
        VkPipelineLayout PipelineLayout = VK_NULL_HANDLE;
        VkPipeline GraphicsPipeline = VK_NULL_HANDLE;

        VkCommandPool CommandPool = VK_NULL_HANDLE;
        Array<VkCommandBuffer> CommandBuffers;

        Array<VkSemaphore> ImageAvailableSemaphores;
        Array<VkSemaphore> RenderFinishedSemaphores;
        Array<VkFence> InFlightFences;
        Array<VkFence> InFlightImages;
        const uint32_t MaxFramesInFlight = 2;
        uint32_t CurrentFrameIndex = 0;
        bool bFramebufferResized = false;
        bool bShouldSubmitFrame = true;

        Array<const char*> RequiredInstanceLayers;
        Array<const char*> RequiredInstanceExtensions;
        Array<const char*> RequiredDeviceExtensions;
        bool bEnableValidationLayers;

        ImGuiVulkanObjects ImGuiObjects;

        VkClearValue ClearColor;
        uint32_t ImageIndex;
    };
}
