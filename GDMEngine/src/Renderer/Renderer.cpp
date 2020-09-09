#include "Renderer/Renderer.h"

#include <vulkan/vulkan.h>
#include <glfw/glfw3.h>
#include <imgui.h>
#include <glm/glm.hpp>

namespace GDMEngine
{
	void TestLibraries()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		if (GLFWwindow* Window = glfwCreateWindow(800, 600, "Test Window", nullptr, nullptr))
		{
			printf("Glfw working!\n");
			glfwDestroyWindow(Window);
		}
		else
		{
			printf("Glfw not working!\n");
			return;
		}

		if (glfwVulkanSupported())
		{
			VkApplicationInfo AppInfo{};
			AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			AppInfo.pApplicationName = "Test Window";
			AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			AppInfo.pEngineName = "GDMEngine";
			AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			AppInfo.apiVersion = VK_API_VERSION_1_0;

			VkInstance Instance;
			VkInstanceCreateInfo CreateInfo{};
			CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			CreateInfo.pApplicationInfo = &AppInfo;

			uint32_t GlfwExtensionCount = 0;
			const char** GlfwExtensionNames = glfwGetRequiredInstanceExtensions(&GlfwExtensionCount);
			CreateInfo.enabledExtensionCount = GlfwExtensionCount;
			CreateInfo.ppEnabledExtensionNames = GlfwExtensionNames;
			CreateInfo.enabledLayerCount = 0;
			if (vkCreateInstance(&CreateInfo, nullptr, &Instance) == VK_SUCCESS)
			{
				printf("Vulkan working!\n");
				vkDestroyInstance(Instance, nullptr);
			}
			else
			{
				printf("Failed to create a Vulkan instance!\n");
			}
		}
		else
		{
			printf("Vulkan not supported!\n");
		}

		glfwTerminate();

		if (ImGuiContext* Ctx = ImGui::CreateContext())
		{
			printf("ImGUI working!\n");
			ImGui::DestroyContext(Ctx);
		}
		else
		{
			printf("ImGUI not working!\n");
		}
	}
}