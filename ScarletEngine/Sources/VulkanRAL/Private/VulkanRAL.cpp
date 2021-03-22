#include "VulkanRAL.h"

#include "Core.h"
#include "Engine.h"
#include "Window.h"
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#define CHECK_RESULT(Res) check(Res == VK_SUCCESS) 

namespace ScarletEngine
{
	static VKAPI_ATTR VkBool32 VKAPI_CALL ValidationDebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT /* MessageType */,
        const VkDebugUtilsMessengerCallbackDataEXT* CallbackData,
        void* /* UserData */)
	{
		switch (MessageSeverity)
		{
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				SCAR_LOG(LogVerbose, "Validation Layer: %s", CallbackData->pMessage);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				SCAR_LOG(LogInfo, "Validation Layer: %s", CallbackData->pMessage);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				SCAR_LOG(LogWarning, "Validation layer: %s", CallbackData->pMessage);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				SCAR_LOG(LogError, "Validation layer: %s", CallbackData->pMessage);
				break;
			default:
				break;
		}

		return VK_FALSE;
	}

	static VkResult CreateDebugUtilsMessengerEXT(const VkInstance Instance, const VkDebugUtilsMessengerCreateInfoEXT* CreateInfo, const VkAllocationCallbacks* Allocator, VkDebugUtilsMessengerEXT* OutDebugMessenger)
	{
		if (const auto Func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(Instance, "vkCreateDebugUtilsMessengerEXT")))
		{
			return Func(Instance, CreateInfo, Allocator, OutDebugMessenger);
		}

		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	static void DestroyDebugUtilsMessengerEXT(const VkInstance Instance, VkDebugUtilsMessengerEXT DebugMessenger, const VkAllocationCallbacks* Allocator)
	{
		if (const auto Func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(Instance, "vkDestroyDebugUtilsMEssengerEXT")))
		{
			Func(Instance, DebugMessenger, Allocator);
		}
	}
	
	VulkanRAL::VulkanRAL()
		: RAL()
		, SwapchainImageFormat()
		, SwapchainImageExtent()
		, bEnableValidationLayers(true) // todo (VkRAL): For now always enable
	{
		RequiredDeviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	}

	bool VulkanRAL::CheckForLayerSupport(String* OutError) const
	{
		uint32_t InstanceLayersCount;
		vkEnumerateInstanceLayerProperties(&InstanceLayersCount, nullptr);
		Array<VkLayerProperties> AvailableLayers(InstanceLayersCount);
		vkEnumerateInstanceLayerProperties(&InstanceLayersCount, AvailableLayers.data());

		for (const char* Layer : RequiredInstanceLayers)
		{
			auto It = std::find_if(AvailableLayers.begin(), AvailableLayers.end(), [Layer](const VkLayerProperties& LayerProps)
			{
				return std::strcmp(Layer, LayerProps.layerName) == 0;
			});
			
			if (It == AvailableLayers.end())
			{
				if (OutError)
				{
					char Buffer[256];
					std::snprintf(Buffer, 256, "Instance layers requested but not available '%s'!", Layer);
					*OutError = String(Buffer);
				}
				return false;
			}
		}

		return true;
	}

	void VulkanRAL::CreateInstance()
	{
		// Before creating the instance, ensure we can properly support all required extensions
		String Error;
		if (!CheckForLayerSupport(&Error))
		{
			SCAR_LOG(LogError, "%s", Error.c_str());
			check(false);
		}
		
		VkApplicationInfo AppInfo{};
		AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		AppInfo.pApplicationName = "Scarlet Engine";
		AppInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
		AppInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
		AppInfo.pEngineName = "Scarlet Engine";
		AppInfo.apiVersion = VK_API_VERSION_1_1;

		VkInstanceCreateInfo InstInfo{};
		InstInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		InstInfo.pApplicationInfo = &AppInfo;

		VkDebugUtilsMessengerCreateInfoEXT DebugCreateInfo{};
		if (bEnableValidationLayers)
		{
			RequiredInstanceLayers.push_back("VK_LAYER_KHRONOS_validation");
			RequiredInstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

			DebugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			DebugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			DebugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			DebugCreateInfo.pfnUserCallback = ValidationDebugCallback;
			DebugCreateInfo.pUserData = nullptr;

			InstInfo.pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&DebugCreateInfo);
		}

		uint32_t GlfwExtensionCount = 0;
		const char** GlfwExtensions = glfwGetRequiredInstanceExtensions(&GlfwExtensionCount);

		for (uint32_t i = 0; i < GlfwExtensionCount; ++i)
		{
			RequiredInstanceExtensions.push_back(GlfwExtensions[i]);
		}
		
		InstInfo.enabledExtensionCount = static_cast<uint32_t>(RequiredInstanceExtensions.size());
		InstInfo.ppEnabledExtensionNames = RequiredInstanceExtensions.data();

		InstInfo.enabledLayerCount = static_cast<uint32_t>(RequiredInstanceLayers.size());
		InstInfo.ppEnabledLayerNames = RequiredInstanceLayers.data();
		
		CHECK_RESULT(vkCreateInstance(&InstInfo, nullptr, &Instance));
	}
	
	void VulkanRAL::SetupDebugMessenger()
	{
		VkDebugUtilsMessengerCreateInfoEXT CreateInfo{};
		CreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		CreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		CreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		CreateInfo.pfnUserCallback = ValidationDebugCallback;
		CreateInfo.pUserData = nullptr;

		CHECK_RESULT(CreateDebugUtilsMessengerEXT(Instance, &CreateInfo, nullptr, &DebugMessenger));
	}

	void VulkanRAL::CreateSurface()
	{
		ApplicationWindow* AppWindow = GEngine->GetApplicationWindow();
		GLFWwindow* WindowHandle = static_cast<GLFWwindow*>(AppWindow->GetWindowHandle());

		CHECK_RESULT(glfwCreateWindowSurface(Instance, WindowHandle, nullptr, &Surface));
	}
	
	VulkanRAL::QueueFamilyIndices VulkanRAL::FindQueueFamilies(const VkPhysicalDevice Device) const
	{
		QueueFamilyIndices Result{};

		uint32_t QueueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, nullptr);
		Array<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, QueueFamilies.data());

		uint32_t Index = 0;
		for (const VkQueueFamilyProperties& QueueFamily : QueueFamilies)
		{
			if (QueueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				Result.GraphicsFamily = Index;
			}

			VkBool32 bHasPresentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(Device, Index, Surface, &bHasPresentSupport);

			if (bHasPresentSupport)
			{
				Result.PresentFamily = Index;
			}
			
			if (Result.IsComplete())
			{
				break;
			}
			++Index;
		}
		
		return Result;		
	}

	VulkanRAL::SwapchainSupportDetails VulkanRAL::QuerySwapchainSupport(const VkPhysicalDevice Device) const
	{
		SwapchainSupportDetails Details{};

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device, Surface, &Details.Capabilities);

		uint32_t FormatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(Device, Surface, &FormatCount, nullptr);
		if (FormatCount != 0)
		{
			Details.Formats.resize(FormatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(Device, Surface, &FormatCount, Details.Formats.data());
		}

		uint32_t PresentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(Device, Surface, &PresentModeCount, nullptr);
		if (PresentModeCount != 0)
		{
			Details.PresentModes.resize(PresentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(Device, Surface, &PresentModeCount, Details.PresentModes.data());
		}
		
		return Details;
	}
	
	bool VulkanRAL::CheckForDeviceExtensionSupport(const VkPhysicalDevice Device, String* OutError) const
	{
		uint32_t ExtensionCount = 0;
		vkEnumerateDeviceExtensionProperties(Device, nullptr, &ExtensionCount, nullptr);
		Array<VkExtensionProperties> AvailableExtensions(ExtensionCount);
		vkEnumerateDeviceExtensionProperties(Device, nullptr, &ExtensionCount, AvailableExtensions.data());

		Set<String> RequiredExtensions(RequiredDeviceExtensions.begin(), RequiredDeviceExtensions.end());

		for (const auto& Extension : AvailableExtensions)
		{
			RequiredExtensions.erase(Extension.extensionName);
		}

		if (!RequiredExtensions.empty())
		{
			if (OutError)
			{
				StringStream ErrorMessage;
				ErrorMessage << "Device extensions requested but not available ";

				for (const String& RequiredExtension : RequiredExtensions)
				{
					ErrorMessage << RequiredExtension;
				}
				*OutError = ErrorMessage.str();
			}
			
			return false;
		}

		return true;
	}

	bool VulkanRAL::IsPhysicalDeviceSuitable(const VkPhysicalDevice Device) const
	{
		const QueueFamilyIndices Indices = FindQueueFamilies(Device);
		const bool bExtensionsSupported = CheckForDeviceExtensionSupport(Device);

		const SwapchainSupportDetails SwapchainSupport = QuerySwapchainSupport(Device);
		const bool bSwapchainAdequate = !SwapchainSupport.Formats.empty() && !SwapchainSupport.PresentModes.empty();
		
		return Indices.IsComplete() && bExtensionsSupported && bSwapchainAdequate;
	}
	
	void VulkanRAL::PickPhysicalDevice()
	{
		uint32_t DeviceCount = 0;
		vkEnumeratePhysicalDevices(Instance, &DeviceCount, nullptr);
		Array<VkPhysicalDevice> Devices(DeviceCount);
		vkEnumeratePhysicalDevices(Instance, &DeviceCount, Devices.data());

		for (const VkPhysicalDevice& PhysDevice : Devices)
		{
			if (IsPhysicalDeviceSuitable(PhysDevice))
			{
				PhysicalDevice = PhysDevice;
				break;
			}
		}

		check(PhysicalDevice != VK_NULL_HANDLE);
	}

	void VulkanRAL::CreateLogicalDevice()
	{
		QueueFamilyIndices Indices = FindQueueFamilies(PhysicalDevice);

		Array<VkDeviceQueueCreateInfo> QueueCreateInfos;
		Set<uint32_t> UniqueQueueFamilies = { Indices.GraphicsFamily.value(), Indices.PresentFamily.value() };

		float QueuePriority = 1.f;
		for (uint32_t QueueFamily : UniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo QueueCreateInfo{};
			QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			QueueCreateInfo.queueFamilyIndex = QueueFamily;
			QueueCreateInfo.queueCount = 1;
			QueueCreateInfo.pQueuePriorities = &QueuePriority;
			
			QueueCreateInfos.push_back(QueueCreateInfo);
		}
		
		VkPhysicalDeviceFeatures DeviceFeatures{};

		VkDeviceCreateInfo CreateInfo{};
		CreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		CreateInfo.queueCreateInfoCount = QueueCreateInfos.size();
		CreateInfo.pQueueCreateInfos = QueueCreateInfos.data();
		CreateInfo.pEnabledFeatures = &DeviceFeatures;
		CreateInfo.enabledExtensionCount = RequiredDeviceExtensions.size();
		CreateInfo.ppEnabledExtensionNames = RequiredDeviceExtensions.data();
		
		if (bEnableValidationLayers)
		{
			CreateInfo.enabledLayerCount = RequiredInstanceLayers.size();
			CreateInfo.ppEnabledLayerNames = RequiredInstanceLayers.data();
		}
		else
		{
			CreateInfo.enabledLayerCount = 0;
		}

		CHECK_RESULT(vkCreateDevice(PhysicalDevice, &CreateInfo, nullptr, &LogicalDevice));

		vkGetDeviceQueue(LogicalDevice, Indices.GraphicsFamily.value(), 0, &GraphicsQueue);
		vkGetDeviceQueue(LogicalDevice, Indices.PresentFamily.value(), 0, &PresentQueue);
	}

	VkSurfaceFormatKHR VulkanRAL::ChooseSwapchainSurfaceFormat(const Array<VkSurfaceFormatKHR>& AvailableFormats)
	{
		for (const VkSurfaceFormatKHR& AvailableFormat : AvailableFormats)
		{
			if (AvailableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && AvailableFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
			{
				return AvailableFormat;
			}
		}
		
		return AvailableFormats[0];
	}

	VkPresentModeKHR VulkanRAL::ChooseSwapchainPresentMode(const Array<VkPresentModeKHR>& AvailablePresentModes)
	{
		for (const VkPresentModeKHR& AvailablePresentMode : AvailablePresentModes)
		{
			if (AvailablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return AvailablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanRAL::ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& Capabilities)
	{
		if (Capabilities.currentExtent.width != UINT32_MAX)
		{
			return Capabilities.currentExtent;
		}
		else
		{
			ApplicationWindow* AppWindow = GEngine->GetApplicationWindow();
			GLFWwindow* WindowHandle = static_cast<GLFWwindow*>(AppWindow->GetWindowHandle());
			int Width, Height;
			glfwGetFramebufferSize(WindowHandle, &Width, &Height);

			VkExtent2D ActualExtent = {
				static_cast<uint32_t>(Width),
					static_cast<uint32_t>(Height)
			};

			ActualExtent.width = std::max(Capabilities.minImageExtent.width, std::min(Capabilities.maxImageExtent.width, ActualExtent.width));
			ActualExtent.height = std::max(Capabilities.minImageExtent.height, std::min(Capabilities.maxImageExtent.height, ActualExtent.height));

			return ActualExtent;
		}
	}

	void VulkanRAL::CreateSwapchain()
	{
		const SwapchainSupportDetails SupportDetails = QuerySwapchainSupport(PhysicalDevice);
		const VkSurfaceFormatKHR SurfaceFormat = ChooseSwapchainSurfaceFormat(SupportDetails.Formats);
		const VkPresentModeKHR PresentMode = ChooseSwapchainPresentMode(SupportDetails.PresentModes);
		const VkExtent2D Extent = ChooseSwapchainExtent(SupportDetails.Capabilities);

		uint32_t ImageCount = SupportDetails.Capabilities.minImageCount + 1;
		if (SupportDetails.Capabilities.maxImageCount > 0 && ImageCount > SupportDetails.Capabilities.maxImageCount)
		{
			ImageCount = SupportDetails.Capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR CreateInfo{};
		CreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		CreateInfo.surface = Surface;
		CreateInfo.minImageCount = ImageCount;
		CreateInfo.imageFormat = SurfaceFormat.format;
		CreateInfo.imageColorSpace = SurfaceFormat.colorSpace;
		CreateInfo.imageExtent = Extent;
		CreateInfo.imageArrayLayers = 1;
		CreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices Indices = FindQueueFamilies(PhysicalDevice);
		uint32_t QueueFamilyIndices[] = { Indices.GraphicsFamily.value(), Indices.PresentFamily.value() };

		if (Indices.GraphicsFamily != Indices.PresentFamily)
		{
			CreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			CreateInfo.queueFamilyIndexCount = 2;
			CreateInfo.pQueueFamilyIndices = QueueFamilyIndices;
		}
		else
		{
			CreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			CreateInfo.queueFamilyIndexCount = 0;
			CreateInfo.pQueueFamilyIndices = nullptr;
		}

		CreateInfo.preTransform = SupportDetails.Capabilities.currentTransform;
		CreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		CreateInfo.presentMode = PresentMode;
		CreateInfo.clipped = VK_TRUE;
		CreateInfo.oldSwapchain = nullptr;

		check(vkCreateSwapchainKHR(LogicalDevice, &CreateInfo, nullptr, &Swapchain) == VK_SUCCESS);

		SwapchainImageFormat = SurfaceFormat.format;
		SwapchainImageExtent = Extent;
		
		vkGetSwapchainImagesKHR(LogicalDevice, Swapchain, &ImageCount, nullptr);
		SwapchainImages.resize(ImageCount);
		vkGetSwapchainImagesKHR(LogicalDevice, Swapchain, &ImageCount, SwapchainImages.data());
	}

	void VulkanRAL::CreateSwapchainImageViews()
	{
		SwapchainImageViews.resize(SwapchainImages.size());

		for (uint32_t i = 0; i < SwapchainImages.size(); ++i)
		{
			VkImageViewCreateInfo CreateInfo{};
			CreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			CreateInfo.image = SwapchainImages[i];
			CreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			CreateInfo.format = SwapchainImageFormat;
			CreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			CreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			CreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			CreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			CreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			CreateInfo.subresourceRange.baseMipLevel = 0;
			CreateInfo.subresourceRange.levelCount = 1;
			CreateInfo.subresourceRange.baseArrayLayer = 0;
			CreateInfo.subresourceRange.layerCount = 1;

			CHECK_RESULT(vkCreateImageView(LogicalDevice, &CreateInfo, nullptr, &SwapchainImageViews[i]));
		}
	}

	void VulkanRAL::Initialize()
	{
		CreateInstance();
		SetupDebugMessenger();
		CreateSurface();
		PickPhysicalDevice();
		CreateLogicalDevice();
		CreateSwapchain();
		CreateSwapchainImageViews();
	}

	void VulkanRAL::Terminate()
	{
		for (VkImageView& ImageView : SwapchainImageViews)
		{
			vkDestroyImageView(LogicalDevice, ImageView, nullptr);
		}

		for (VkImage& Image: SwapchainImages)
		{
			vkDestroyImage(LogicalDevice, Image, nullptr);
		}
		
		vkDestroySwapchainKHR(LogicalDevice, Swapchain, nullptr);
		
		vkDestroyDevice(LogicalDevice, nullptr);
		
		if (bEnableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(Instance, DebugMessenger, nullptr);
		}

		vkDestroySurfaceKHR(Instance, Surface, nullptr);
		
		vkDestroyInstance(Instance, nullptr);
	}

	GPUInfo VulkanRAL::GetGPUInfo() const
	{
		GPUInfo Result{};

		
		return Result;
	}

	void VulkanRAL::SetClearColorCommand(const glm::vec4& ClearColor) const
	{

	}

	void VulkanRAL::ClearCommand(bool bColor, bool bDepth, bool bStencil) const
	{

	}

	void VulkanRAL::DrawVertexArray(const RALVertexArray* VA) const
	{

	}

	RALFramebuffer* VulkanRAL::CreateFramebuffer(uint32_t Width, uint32_t Height, uint32_t Samples) const
	{
		return nullptr;
	}

	RALTexture2D* VulkanRAL::CreateTexture2D(const WeakPtr<TextureHandle>& AssetHandle) const
	{
		return nullptr;
	}

	RALGpuBuffer* VulkanRAL::CreateBuffer(uint32_t Size, RALBufferUsage Usage) const
	{
		return nullptr;
	}

	RALVertexArray* VulkanRAL::CreateVertexArray(const RALGpuBuffer* VB, const RALGpuBuffer* IB) const
	{
		return nullptr;
	}

	RALShader* VulkanRAL::CreateShader(RALShaderStage Stage, const String& ShaderPath) const
	{
		return nullptr;
	}

	RALShaderProgram* VulkanRAL::CreateShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader, RALShader* InGeometryShader, RALShader* InComputeShader) const
	{
		return nullptr;
	}
}