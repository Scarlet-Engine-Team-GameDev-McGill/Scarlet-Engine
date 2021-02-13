#include "Window.h"

#include "Core.h"
#include "AssetManager.h"
#include <GLFW/glfw3.h>

namespace ScarletEngine
{
	static void WindowCloseCallback(GLFWwindow* WindowHandle)
	{
		ApplicationWindow* AppWindow = (ApplicationWindow*)glfwGetWindowUserPointer(WindowHandle);
		
		AppWindow->OnWindowCloseEvent().Broadcast();
	}

	static void WindowResizeCallback(GLFWwindow* WindowHandle, int Width, int Height)
	{
		ApplicationWindow* AppWindow = (ApplicationWindow*)glfwGetWindowUserPointer(WindowHandle);

		AppWindow->OnWindowResizeEvent().Broadcast(Width, Height);
	}

	ApplicationWindow::ApplicationWindow(uint32_t InWidth, uint32_t InHeight, const String& InWindowTitle)
		: Width(InWidth)
		, Height(InHeight)
		, WindowTitle(InWindowTitle)
	{
		check(Width != 0 && Height != 0);

		glfwInit();
#ifdef RAL_USE_OPENGL
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
#elif defined RAL_USE_VULKAN
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif

		glfwWindowHint(GLFW_SAMPLES, 4);

		WindowHandle = glfwCreateWindow(Width, Height, WindowTitle.c_str(), nullptr, nullptr);
		if (WindowHandle == nullptr)
		{
			glfwTerminate();
			check(false);
		}

		glfwSetWindowUserPointer((GLFWwindow*)WindowHandle, this);

		// Set the window icon;
		SharedPtr<TextureHandle> LogoTex = AssetManager::LoadTextureFile("/ScarletEngine/Content/scarlet_logo.png");
		GLFWimage Image;
		Image.pixels = LogoTex->PixelDataBuffer;
		Image.width = LogoTex->Width;
		Image.height = LogoTex->Height;

		glfwSetWindowIcon((GLFWwindow*)WindowHandle, 1, &Image);

		glfwSetWindowCloseCallback((GLFWwindow*)WindowHandle, WindowCloseCallback);
		glfwSetFramebufferSizeCallback((GLFWwindow*)WindowHandle, WindowResizeCallback);
	}


	void ApplicationWindow::SwapBuffer()
	{
		glfwSwapBuffers((GLFWwindow*)WindowHandle);
	}

	void ApplicationWindow::PollEvents()
	{
		glfwPollEvents();
	}
}