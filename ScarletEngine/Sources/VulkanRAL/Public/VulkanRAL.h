#pragma once

#include "RAL.h"
#include <optional>

// #todo: Currently leaking this include, need to find a way to move this
#include <vulkan/vulkan.h>

namespace ScarletEngine
{	
	class VulkanRAL : public RAL
	{
	public:
		VulkanRAL();
		virtual ~VulkanRAL() {}

		// Begin RAL Interface
		virtual void Initialize() override;
		virtual void Terminate() override;
		virtual const char* GetBackendName() const override { return "Vulkan"; }
		virtual GPUInfo GetGPUInfo() const override;

		virtual void SetClearColorCommand(const glm::vec4& ClearColor) const override;
		virtual void ClearCommand(bool bColor, bool bDepth, bool bStencil) const override;
		virtual void DrawVertexArray(const RALVertexArray* VA) const override;

		virtual RALFramebuffer* CreateFramebuffer(uint32_t Width, uint32_t Height, uint32_t Samples) const override;
		virtual RALTexture2D* CreateTexture2D(const WeakPtr<TextureHandle>& AssetHandle) const override;
		virtual RALGpuBuffer* CreateBuffer(uint32_t Size, RALBufferUsage Usage) const override;
		virtual RALVertexArray* CreateVertexArray(const RALGpuBuffer* VB, const RALGpuBuffer* IB) const override;
		virtual RALShader* CreateShader(RALShaderStage Stage, const String& ShaderPath) const override;
		virtual RALShaderProgram* CreateShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader, RALShader* InGeometryShader, RALShader* InComputeShader) const override;
		// End RAL Interface

		void SubmitFrame();
	private:
		bool CheckForLayerSupport(String* OutError = nullptr) const;
		void CreateInstance();
		void SetupDebugMessenger();

		struct QueueFamilyIndices
		{
			std::optional<uint32_t> GraphicsFamily;
			std::optional<uint32_t> PresentFamily;

			bool IsComplete() const { return GraphicsFamily.has_value() && PresentFamily.has_value(); }
		};

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

		void CreateSyncObjects();
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
		
		Array<const char*> RequiredInstanceLayers;
		Array<const char*> RequiredInstanceExtensions;
		Array<const char*> RequiredDeviceExtensions;
		bool bEnableValidationLayers;
	};
}
