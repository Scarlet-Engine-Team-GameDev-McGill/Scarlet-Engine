#include "Window.h"

#include "Core.h"
#include "AssetManager.h"
#include "GLFW/glfw3.h"
#include "InputManager.h"

namespace ScarletEngine
{
	static void WindowCloseCallback(GLFWwindow* WindowHandle)
	{
		ApplicationWindow* AppWindow = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(WindowHandle));
		
		AppWindow->OnWindowClose.Broadcast();
	}

	static void WindowResizeCallback(GLFWwindow* WindowHandle, int Width, int Height)
	{
		ApplicationWindow* AppWindow = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(WindowHandle));

		AppWindow->OnWindowResize.Broadcast(Width, Height);
	}

	static void KeyCallback(GLFWwindow*, int Key, int, int Action, int Mods)
	{
		if (Action == GLFW_PRESS)
		{
			InputManager::Get().OnKeyDownCallback(static_cast<KeyCode>(Key));
		}
		else if (Action == GLFW_RELEASE)
		{
			InputManager::Get().OnKeyUpCallback(static_cast<KeyCode>(Key));
		}
	}

	static void CursorPosCallback(GLFWwindow*, double Xpos, double Ypos)
	{
		InputManager::Get().OnMouseMoveCallback({ Xpos, Ypos});
	}

	static void MouseButtonCallback(GLFWwindow*, int Button, int Action, int Mods)
	{
		if (Action == GLFW_PRESS)
		{
			InputManager::Get().OnMousePressCallback(static_cast<MouseCode>(Button));
		}
		else if (Action == GLFW_RELEASE)
		{
			InputManager::Get().OnMouseReleaseCallback(static_cast<MouseCode>(Button));
		}
	}

	ApplicationWindow::ApplicationWindow(uint32_t InWidth, uint32_t InHeight, const String& InWindowTitle)
		: WindowHandle(nullptr)
		, Width(InWidth)
		, Height(InHeight)
		, WindowTitle(InWindowTitle)
		, LastCursorXPos(0)
		, LastCursorYPos(0)
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
		glfwMaximizeWindow((GLFWwindow*)WindowHandle);

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
		glfwSetKeyCallback((GLFWwindow*)WindowHandle, KeyCallback);
		glfwSetCursorPosCallback((GLFWwindow*)WindowHandle, CursorPosCallback);
		glfwSetMouseButtonCallback((GLFWwindow*)WindowHandle, MouseButtonCallback);

		OnCursorMove.Bind([this](CursorMoveEvent CursorEvent) { LastCursorXPos += CursorEvent.DeltaX; LastCursorYPos += CursorEvent.DeltaY; });
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