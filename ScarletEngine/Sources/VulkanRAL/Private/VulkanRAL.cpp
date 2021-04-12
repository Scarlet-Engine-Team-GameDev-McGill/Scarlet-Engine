#include "VulkanRAL.h"

#include "Core.h"
#include "Engine.h"
#include "Window.h"
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <VulkanResources.h>

#include "VulkanCommandList.h"

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
            //SCAR_LOG(LogVerbose, "Validation Layer: %s", CallbackData->pMessage);
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

    static VkResult CreateDebugUtilsMessengerEXT(const VkInstance Instance,
                                                 const VkDebugUtilsMessengerCreateInfoEXT* CreateInfo,
                                                 const VkAllocationCallbacks* Allocator,
                                                 VkDebugUtilsMessengerEXT* OutDebugMessenger)
    {
        if (const auto Func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(
            Instance, "vkCreateDebugUtilsMessengerEXT")))
        {
            return Func(Instance, CreateInfo, Allocator, OutDebugMessenger);
        }

        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    static void DestroyDebugUtilsMessengerEXT(const VkInstance Instance, VkDebugUtilsMessengerEXT DebugMessenger,
                                              const VkAllocationCallbacks* Allocator)
    {
        if (const auto Func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(
            Instance, "vkDestroyDebugUtilsMessengerEXT")))
        {
            Func(Instance, DebugMessenger, Allocator);
        }
    }

    VulkanRAL::VulkanRAL()
        : RAL()
          , SwapchainImageFormat()
          , SwapchainImageExtent()
          , bEnableValidationLayers(true) // todo (VkRAL): For now always enable
          , ClearColor({ 0.1f, 0.1f, 0.1f, 1.f })
          , ImageIndex(-1)
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
            auto It = std::find_if(AvailableLayers.begin(), AvailableLayers.end(),
                                   [Layer](const VkLayerProperties& LayerProps)
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

        VkApplicationInfo AppInfo {};
        AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        AppInfo.pApplicationName = "Scarlet Engine";
        AppInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
        AppInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
        AppInfo.pEngineName = "Scarlet Engine";
        AppInfo.apiVersion = VK_API_VERSION_1_1;

        VkInstanceCreateInfo InstInfo {};
        InstInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        InstInfo.pApplicationInfo = &AppInfo;

        VkDebugUtilsMessengerCreateInfoEXT DebugCreateInfo {};
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
        VkDebugUtilsMessengerCreateInfoEXT CreateInfo {};
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
        QueueFamilyIndices Result {};

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
        SwapchainSupportDetails Details {};

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
            VkDeviceQueueCreateInfo QueueCreateInfo {};
            QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            QueueCreateInfo.queueFamilyIndex = QueueFamily;
            QueueCreateInfo.queueCount = 1;
            QueueCreateInfo.pQueuePriorities = &QueuePriority;

            QueueCreateInfos.push_back(QueueCreateInfo);
        }

        VkPhysicalDeviceFeatures DeviceFeatures {};

        VkDeviceCreateInfo CreateInfo {};
        CreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        CreateInfo.queueCreateInfoCount = static_cast<uint32_t>(QueueCreateInfos.size());
        CreateInfo.pQueueCreateInfos = QueueCreateInfos.data();
        CreateInfo.pEnabledFeatures = &DeviceFeatures;
        CreateInfo.enabledExtensionCount = static_cast<uint32_t>(RequiredDeviceExtensions.size());
        CreateInfo.ppEnabledExtensionNames = RequiredDeviceExtensions.data();

        if (bEnableValidationLayers)
        {
            CreateInfo.enabledLayerCount = static_cast<uint32_t>(RequiredInstanceLayers.size());
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
            if (AvailableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && AvailableFormat.colorSpace ==
                VK_COLORSPACE_SRGB_NONLINEAR_KHR)
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

            ActualExtent.width = std::max(Capabilities.minImageExtent.width,
                                          std::min(Capabilities.maxImageExtent.width, ActualExtent.width));
            ActualExtent.height = std::max(Capabilities.minImageExtent.height,
                                           std::min(Capabilities.maxImageExtent.height, ActualExtent.height));

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

        VkSwapchainCreateInfoKHR CreateInfo {};
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
            VkImageViewCreateInfo CreateInfo {};
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

    Array<char> VulkanRAL::ReadShaderFile(const String& Filename)
    {
        std::ifstream ShaderFile(Filename, std::ios::ate | std::ios::binary);

        if (!ShaderFile.is_open())
        {
            SCAR_LOG(LogError, "Unable to open shader file (%s)", Filename.c_str());
            return Array<char>();
        }

        const size_t FileSize = static_cast<size_t>(ShaderFile.tellg());
        Array<char> Buffer(FileSize);

        ShaderFile.seekg(0);
        ShaderFile.read(Buffer.data(), FileSize);
        ShaderFile.close();
        return Buffer;
    }

    VkShaderModule VulkanRAL::CreateShaderModule(const Array<char>& Code) const
    {
        VkShaderModuleCreateInfo CreateInfo {};
        CreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        CreateInfo.codeSize = Code.size();
        CreateInfo.pCode = reinterpret_cast<const uint32_t*>(Code.data());

        VkShaderModule ShaderModule;
        CHECK_RESULT(vkCreateShaderModule(LogicalDevice, &CreateInfo, nullptr, &ShaderModule));
        return ShaderModule;
    }

    void VulkanRAL::CreateRenderPass()
    {
        VkAttachmentDescription ColorAttachment {};
        ColorAttachment.format = SwapchainImageFormat;
        ColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        ColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        ColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        ColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        ColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference ColorAttachmentRef {};
        ColorAttachmentRef.attachment = 0;
        ColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription Subpass {};
        Subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        Subpass.colorAttachmentCount = 1;
        Subpass.pColorAttachments = &ColorAttachmentRef;

        VkSubpassDependency Dependency {};
        Dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        Dependency.dstSubpass = 0;
        Dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        Dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        Dependency.srcAccessMask = 0;
        Dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo RenderPassInfo {};
        RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        RenderPassInfo.attachmentCount = 1;
        RenderPassInfo.pAttachments = &ColorAttachment;
        RenderPassInfo.subpassCount = 1;
        RenderPassInfo.pSubpasses = &Subpass;
        RenderPassInfo.dependencyCount = 1;
        RenderPassInfo.pDependencies = &Dependency;

        CHECK_RESULT(vkCreateRenderPass(LogicalDevice, &RenderPassInfo, nullptr, &RenderPass));
    }

    void VulkanRAL::CreateGraphicsPipeline()
    {
        const Array<char> VertexShaderCode = ReadShaderFile("../ScarletEngine/Shaders/vert.spv");
        const Array<char> FragmentShaderCode = ReadShaderFile("../ScarletEngine/Shaders/frag.spv");

        const VkShaderModule VertexShaderModule = CreateShaderModule(VertexShaderCode);
        const VkShaderModule FragmentShaderModule = CreateShaderModule(FragmentShaderCode);

        VkPipelineShaderStageCreateInfo VertexShaderStageInfo {};
        VertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        VertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        VertexShaderStageInfo.module = VertexShaderModule;
        VertexShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo FragmentShaderStageInfo {};
        FragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        FragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        FragmentShaderStageInfo.module = FragmentShaderModule;
        FragmentShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo ShaderStages[] = { VertexShaderStageInfo, FragmentShaderStageInfo };

        const VkVertexInputBindingDescription BindingDescription = VulkanVertexArray::GetBindingDescription();
        const Array<VkVertexInputAttributeDescription> AttributeDescriptions =
            VulkanVertexArray::GetAttributeDescription();

        VkPipelineVertexInputStateCreateInfo VertexInputInfo {};
        VertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        VertexInputInfo.vertexBindingDescriptionCount = 1;
        VertexInputInfo.pVertexBindingDescriptions = &BindingDescription;
        VertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(AttributeDescriptions.size());
        VertexInputInfo.pVertexAttributeDescriptions = AttributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo InputAssembly {};
        InputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        InputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        InputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport Viewport {};
        Viewport.x = 0.f;
        Viewport.y = 0.f;
        Viewport.width = static_cast<float>(SwapchainImageExtent.width);
        Viewport.height = static_cast<float>(SwapchainImageExtent.height);
        Viewport.minDepth = 0.f;
        Viewport.maxDepth = 1.f;

        VkRect2D Scissor {};
        Scissor.offset = { 0, 0 };
        Scissor.extent = SwapchainImageExtent;

        VkPipelineViewportStateCreateInfo ViewportState {};
        ViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        ViewportState.viewportCount = 1;
        ViewportState.pViewports = &Viewport;
        ViewportState.scissorCount = 1;
        ViewportState.pScissors = &Scissor;

        VkPipelineRasterizationStateCreateInfo Rasterizer {};
        Rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        Rasterizer.depthClampEnable = VK_FALSE;
        Rasterizer.rasterizerDiscardEnable = VK_FALSE;
        Rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        Rasterizer.lineWidth = 1.f;
        Rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        Rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        Rasterizer.depthBiasEnable = VK_FALSE;
        Rasterizer.depthBiasConstantFactor = 0.f;
        Rasterizer.depthBiasClamp = 0.f;
        Rasterizer.depthBiasSlopeFactor = 0.f;

        VkPipelineMultisampleStateCreateInfo MultisampleState {};
        MultisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        MultisampleState.sampleShadingEnable = VK_FALSE;
        MultisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        MultisampleState.minSampleShading = 1.f;
        MultisampleState.pSampleMask = nullptr;
        MultisampleState.alphaToCoverageEnable = VK_FALSE;
        MultisampleState.alphaToOneEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState ColorBlendAttachmentState {};
        ColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        ColorBlendAttachmentState.blendEnable = VK_FALSE;
        ColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        ColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        ColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
        ColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        ColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        ColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo ColorBlending {};
        ColorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        ColorBlending.logicOpEnable = VK_FALSE;
        ColorBlending.logicOp = VK_LOGIC_OP_COPY;
        ColorBlending.attachmentCount = 1;
        ColorBlending.pAttachments = &ColorBlendAttachmentState;
        ColorBlending.blendConstants[0] = 0.f;
        ColorBlending.blendConstants[1] = 0.f;
        ColorBlending.blendConstants[2] = 0.f;
        ColorBlending.blendConstants[3] = 0.f;

        VkPipelineLayoutCreateInfo PipelineLayoutInfo {};
        PipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        PipelineLayoutInfo.setLayoutCount = 0;
        PipelineLayoutInfo.pSetLayouts = nullptr;
        PipelineLayoutInfo.pushConstantRangeCount = 0;
        PipelineLayoutInfo.pPushConstantRanges = nullptr;

        CHECK_RESULT(vkCreatePipelineLayout(LogicalDevice, &PipelineLayoutInfo, nullptr, &PipelineLayout));

        VkGraphicsPipelineCreateInfo PipelineInfo {};
        PipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        PipelineInfo.stageCount = 2;
        PipelineInfo.pStages = ShaderStages;
        PipelineInfo.pVertexInputState = &VertexInputInfo;
        PipelineInfo.pInputAssemblyState = &InputAssembly;
        PipelineInfo.pViewportState = &ViewportState;
        PipelineInfo.pRasterizationState = &Rasterizer;
        PipelineInfo.pMultisampleState = &MultisampleState;
        PipelineInfo.pDepthStencilState = nullptr;
        PipelineInfo.pColorBlendState = &ColorBlending;
        PipelineInfo.pDynamicState = nullptr;
        PipelineInfo.layout = PipelineLayout;
        PipelineInfo.renderPass = RenderPass;
        PipelineInfo.subpass = 0;
        PipelineInfo.basePipelineHandle = nullptr;
        PipelineInfo.basePipelineIndex = -1;

        CHECK_RESULT(
            vkCreateGraphicsPipelines(LogicalDevice, VK_NULL_HANDLE, 1, &PipelineInfo, nullptr, &GraphicsPipeline));

        vkDestroyShaderModule(LogicalDevice, FragmentShaderModule, nullptr);
        vkDestroyShaderModule(LogicalDevice, VertexShaderModule, nullptr);
    }

    void VulkanRAL::CreateFramebuffers()
    {
        SwapchainFramebuffers.resize(SwapchainImageViews.size());

        for (size_t i = 0; i < SwapchainImageViews.size(); ++i)
        {
            VkImageView Attachments[] = { SwapchainImageViews[i] };

            VkFramebufferCreateInfo FramebufferInfo {};
            FramebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            FramebufferInfo.renderPass = RenderPass;
            FramebufferInfo.attachmentCount = 1;
            FramebufferInfo.pAttachments = Attachments;
            FramebufferInfo.width = SwapchainImageExtent.width;
            FramebufferInfo.height = SwapchainImageExtent.height;
            FramebufferInfo.layers = 1;

            CHECK_RESULT(vkCreateFramebuffer(LogicalDevice, &FramebufferInfo, nullptr, &SwapchainFramebuffers[i]));
        }
    }

    void VulkanRAL::CreateCommandPool()
    {
        const QueueFamilyIndices QueueFamilies = FindQueueFamilies(PhysicalDevice);

        VkCommandPoolCreateInfo PoolInfo {};
        PoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        PoolInfo.queueFamilyIndex = QueueFamilies.GraphicsFamily.value();
        PoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        CHECK_RESULT(vkCreateCommandPool(LogicalDevice, &PoolInfo, nullptr, &CommandPool));
    }

    void VulkanRAL::CreateCommandBuffers()
    {
        CommandBuffers.resize(SwapchainFramebuffers.size());
        VkCommandBufferAllocateInfo AllocInfo {};
        AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        AllocInfo.commandPool = CommandPool;
        AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        AllocInfo.commandBufferCount = static_cast<uint32_t>(CommandBuffers.size());

        CHECK_RESULT(vkAllocateCommandBuffers(LogicalDevice, &AllocInfo, CommandBuffers.data()));
    }

    void VulkanRAL::BeginRenderPassCommandBuff(VkCommandBuffer& CmdBuff)
    {
        VkRenderPassBeginInfo RenderPassBeginInfo {};
        RenderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        RenderPassBeginInfo.renderPass = RenderPass;
        RenderPassBeginInfo.framebuffer = SwapchainFramebuffers[ImageIndex];
        RenderPassBeginInfo.renderArea.offset = { 0, 0 };
        RenderPassBeginInfo.renderArea.extent = SwapchainImageExtent;

        RenderPassBeginInfo.clearValueCount = 1;
        RenderPassBeginInfo.pClearValues = &ClearColor;

        vkCmdBeginRenderPass(CmdBuff, &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(CmdBuff, VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipeline);
    }

    void VulkanRAL::EndRenderPassCommandBuff(VkCommandBuffer& CmdBuff)
    {
        vkCmdEndRenderPass(CmdBuff);
    }

    void VulkanRAL::CreateSyncObjects()
    {
        ImageAvailableSemaphores.resize(MaxFramesInFlight);
        RenderFinishedSemaphores.resize(MaxFramesInFlight);
        InFlightFences.resize(MaxFramesInFlight);
        InFlightImages.resize(SwapchainImages.size(), nullptr);

        VkSemaphoreCreateInfo SemaphoreCreateInfo {};
        SemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo FenceCreateInfo {};
        FenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        FenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MaxFramesInFlight; ++i)
        {
            CHECK_RESULT(vkCreateSemaphore(LogicalDevice, &SemaphoreCreateInfo, nullptr, &ImageAvailableSemaphores[i]));
            CHECK_RESULT(vkCreateSemaphore(LogicalDevice, &SemaphoreCreateInfo, nullptr, &RenderFinishedSemaphores[i]));
            CHECK_RESULT(vkCreateFence(LogicalDevice, &FenceCreateInfo, nullptr, &InFlightFences[i]));
        }
    }

    void VulkanRAL::CleanupSwapchain()
    {
        vkDeviceWaitIdle(LogicalDevice);

        for (VkFramebuffer& Framebuffer : SwapchainFramebuffers)
        {
            vkDestroyFramebuffer(LogicalDevice, Framebuffer, nullptr);
        }

        vkDestroyPipeline(LogicalDevice, GraphicsPipeline, nullptr);

        vkDestroyPipelineLayout(LogicalDevice, PipelineLayout, nullptr);

        vkDestroyRenderPass(LogicalDevice, RenderPass, nullptr);

        for (VkImageView& ImageView : SwapchainImageViews)
        {
            vkDestroyImageView(LogicalDevice, ImageView, nullptr);
        }

        vkDestroySwapchainKHR(LogicalDevice, Swapchain, nullptr);
    }

    void VulkanRAL::RebuildSwapchain()
    {
        ImageIndex = -1;
        // We don't need to recruit the swapchain for a minimized window.
        int Width = 0, Height = 0;
        const ApplicationWindow* AppWindow = GEngine->GetApplicationWindow();
        glfwGetFramebufferSize((GLFWwindow*)AppWindow->GetWindowHandle(), &Width, &Height);
        if (Width == 0 || Height == 0)
        {
            bShouldSubmitFrame = false;
            return;
        }

        CleanupSwapchain();

        CreateSwapchain();
        CreateSwapchainImageViews();
        CreateRenderPass();
        CreateGraphicsPipeline();
        CreateFramebuffers();

        bFramebufferResized = false;
        bShouldSubmitFrame = true;
    }

    void VulkanRAL::Initialize()
    {
        // Hook into the window framebuffer resize callback
        if (GEngine)
        {
            const ApplicationWindow* AppWindow = GEngine->GetApplicationWindow();
            check(AppWindow != nullptr);
            AppWindow->OnWindowResize.Bind([this](uint32_t, uint32_t)
            {
                bFramebufferResized = true;
            }, this);
        }

        CreateInstance();
        SetupDebugMessenger();
        CreateSurface();
        PickPhysicalDevice();
        CreateLogicalDevice();
        CreateSwapchain();
        CreateSwapchainImageViews();
        CreateRenderPass();
        CreateGraphicsPipeline();
        CreateFramebuffers();
        CreateCommandPool();
        CreateCommandBuffers();
        CreateSyncObjects();
    }

    void VulkanRAL::PreFrame()
    {
        OnScopeExit Scoped([this]()
        {
            CommandListQueue.push(UniquePtr<RALCommandList>(CreateCommandList()));
        });

        if (!bShouldSubmitFrame)
        {
            RebuildSwapchain();
            return;
        }

        vkWaitForFences(LogicalDevice, 1, &InFlightFences[CurrentFrameIndex], VK_TRUE, UINT64_MAX);

        ImageIndex = -1;
        const VkResult Result = vkAcquireNextImageKHR(LogicalDevice, Swapchain, UINT64_MAX,
                                                      ImageAvailableSemaphores[CurrentFrameIndex], nullptr,
                                                      &ImageIndex);
        if (Result == VK_ERROR_OUT_OF_DATE_KHR || Result == VK_SUBOPTIMAL_KHR || bFramebufferResized)
        {
            RebuildSwapchain();
            return;
        }

        if (Result != VK_SUCCESS)
        {
            SCAR_LOG(LogError, "Failed to acquire swapchain image!");
        }
    }

    void VulkanRAL::Submit()
    {
        VulkanCommandList& CmdList = static_cast<VulkanCommandList&>(*CommandListQueue.front().get());

        OnScopeExit Scoped([this]()
        {
            CommandListQueue.pop();
        });

        if (ImageIndex == -1 || CmdList.CmdBuff == VK_NULL_HANDLE || !bShouldSubmitFrame || bFramebufferResized)
        {
            return;
        }

        // Record all commands into the VkCommandBuffer
        {
            VkCommandBufferBeginInfo BeginInfo {};
            BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            BeginInfo.flags = 0;
            BeginInfo.pInheritanceInfo = nullptr;

            CHECK_RESULT(vkBeginCommandBuffer(CmdList.CmdBuff, &BeginInfo));

            BeginRenderPassCommandBuff(CmdList.CmdBuff);

            CmdList.ExecuteAll();

            EndRenderPassCommandBuff(CmdList.CmdBuff);

            CHECK_RESULT(vkEndCommandBuffer(CmdList.CmdBuff));
        }

        if (InFlightImages[ImageIndex] != nullptr)
        {
            vkWaitForFences(LogicalDevice, 1, &InFlightImages[ImageIndex], VK_TRUE, UINT64_MAX);
        }

        InFlightImages[ImageIndex] = InFlightFences[CurrentFrameIndex];

        VkSubmitInfo SubmitInfo {};
        SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore WaitSemaphores[] = { ImageAvailableSemaphores[CurrentFrameIndex] };
        VkPipelineStageFlags WaitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        SubmitInfo.waitSemaphoreCount = 1;
        SubmitInfo.pWaitSemaphores = WaitSemaphores;
        SubmitInfo.pWaitDstStageMask = WaitStages;
        SubmitInfo.commandBufferCount = 1;
        SubmitInfo.pCommandBuffers = &CmdList.CmdBuff;

        VkSemaphore SignalSemaphores[] = { RenderFinishedSemaphores[CurrentFrameIndex] };
        SubmitInfo.signalSemaphoreCount = 1;
        SubmitInfo.pSignalSemaphores = SignalSemaphores;

        vkResetFences(LogicalDevice, 1, &InFlightFences[CurrentFrameIndex]);

        CHECK_RESULT(vkQueueSubmit(GraphicsQueue, 1, &SubmitInfo, InFlightFences[CurrentFrameIndex]));

        VkPresentInfoKHR PresentInfo {};
        PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        PresentInfo.waitSemaphoreCount = 1;
        PresentInfo.pWaitSemaphores = SignalSemaphores;

        VkSwapchainKHR Swapchains[] = { Swapchain };
        PresentInfo.swapchainCount = 1;
        PresentInfo.pSwapchains = Swapchains;
        PresentInfo.pImageIndices = &ImageIndex;
        PresentInfo.pResults = nullptr;

        vkQueuePresentKHR(PresentQueue, &PresentInfo);

        CurrentFrameIndex = (CurrentFrameIndex + 1) % MaxFramesInFlight;
    }

    void VulkanRAL::Terminate()
    {
        vkDeviceWaitIdle(LogicalDevice);

        GEngine->GetApplicationWindow()->OnWindowResize.Unbind(this);

        CleanupSwapchain();

        for (size_t i = 0; i < MaxFramesInFlight; ++i)
        {
            vkDestroySemaphore(LogicalDevice, RenderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(LogicalDevice, ImageAvailableSemaphores[i], nullptr);
            vkDestroyFence(LogicalDevice, InFlightFences[i], nullptr);
        }

        vkDestroyCommandPool(LogicalDevice, CommandPool, nullptr);

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
        GPUInfo Result {};

        return Result;
    }

    RALCommandList* VulkanRAL::CreateCommandList() const
    {
        return ScarNew(VulkanCommandList,
                       ImageIndex != -1 ? CommandBuffers[ImageIndex] : VkCommandBuffer{VK_NULL_HANDLE});
    }

    void VulkanRAL::SetClearColorCmd(const glm::vec4& InClearColor)
    {
        const VkClearValue ClearValue = { InClearColor.x, InClearColor.y, InClearColor.z, InClearColor.a };
        QueueCommand([this, ClearValue](RALCommandList&)
        {
            ClearColor = ClearValue;
        }, "ClearCommand");
    }

    void VulkanRAL::ClearCmd(bool bColor, bool bDepth, bool bStencil)
    {
    }

    void VulkanRAL::DrawVertexArrayCmd(const RALVertexArray* VA)
    {
        QueueCommand([VA](RALCommandList& CmdList)
        {
            VulkanCommandList& VkCmdList = static_cast<VulkanCommandList&>(CmdList);
            VkDeviceSize Offsets[] = { 0 };

            const VulkanGpuBuffer* VB = static_cast<const VulkanGpuBuffer*>(VA->VB);

            VkBuffer VertexBuffers[] = { VB->Buffer };

            vkCmdBindVertexBuffers(VkCmdList.CmdBuff, 0, 1, VertexBuffers, Offsets);

            vkCmdDraw(VkCmdList.CmdBuff, static_cast<uint32_t>(VA->GetCount()), 1, 0, 0);
        });
    }

    RALFramebuffer* VulkanRAL::CreateFramebuffer(uint32_t Width, uint32_t Height, uint32_t Samples)
    {
        return nullptr;
    }

    RALTexture2D* VulkanRAL::CreateTexture2D(const WeakPtr<TextureHandle>& AssetHandle)
    {
        return nullptr;
    }

    RALGpuBuffer* VulkanRAL::CreateBuffer(uint32_t Size, RALBufferType Type, RALBufferUsage Usage,
                                          RALBufferPropertyFlags Properties)
    {
        return ScarNew(VulkanGpuBuffer, PhysicalDevice, LogicalDevice, Size, Type, Usage, Properties);
    }

    RALVertexArray* VulkanRAL::CreateVertexArray(const RALGpuBuffer* VB, const RALGpuBuffer* IB)
    {
        return ScarNew(VulkanVertexArray, VB, IB);
    }

    RALShader* VulkanRAL::CreateShader(RALShaderStage Stage, const String& ShaderPath)
    {
        return nullptr;
    }

    RALShaderProgram* VulkanRAL::CreateShaderProgram(RALShader* InVertexShader, RALShader* InPixelShader,
                                                     RALShader* InGeometryShader, RALShader* InComputeShader)
    {
        return nullptr;
    }
}
