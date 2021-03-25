#include "Window.h"

#include "Core.h"
#include "AssetManager.h"
#include <GLFW/glfw3.h>

namespace ScarletEngine
{
	static void WindowCloseCallback(GLFWwindow* WindowHandle)
	{
		ApplicationWindow* AppWindow = (ApplicationWindow*)glfwGetWindowUserPointer(WindowHandle);
		
		AppWindow->OnWindowClose.Broadcast();
	}

	static void WindowResizeCallback(GLFWwindow* WindowHandle, int Width, int Height)
	{
		ApplicationWindow* AppWindow = (ApplicationWindow*)glfwGetWindowUserPointer(WindowHandle);

		AppWindow->OnWindowResize.Broadcast(Width, Height);
	}

	static void KeyCallback(GLFWwindow* WindowHandle, int Key, int, int Action, int Mods)
	{
		ApplicationWindow* AppWindow = (ApplicationWindow*)glfwGetWindowUserPointer(WindowHandle);

		KeyInputEvent InputEvent{ (uint32_t)Key, (uint32_t)Mods };

		if (Action == GLFW_KEY_DOWN)
		{
			AppWindow->OnKeyDown.Broadcast(InputEvent);
		}
		else if (Action == GLFW_KEY_UP)
		{
			AppWindow->OnKeyUp.Broadcast(InputEvent);
		}
	}

	static void CursorPosCallback(GLFWwindow* WindowHandle, double Xpos, double Ypos)
	{
		ApplicationWindow* AppWindow = (ApplicationWindow*)glfwGetWindowUserPointer(WindowHandle);

		const double DeltaXPos = Xpos - AppWindow->GetLastCursorXPos();
		const double DeltaYPos = Ypos - AppWindow->GetLastCursorYPos();

		CursorMoveEvent MoveEvent{ DeltaXPos, DeltaYPos };

		AppWindow->OnCursorMove.Broadcast(MoveEvent);
	}

	static void MouseButtonCallback(GLFWwindow* WindowHandle, int Button, int Action, int Mods)
	{
		ApplicationWindow* AppWindow = (ApplicationWindow*)glfwGetWindowUserPointer(WindowHandle);

		MouseInputEvent MouseEvent{ (uint32_t)Button, (uint32_t)Mods };

		if (Action == GLFW_PRESS)
		{
			AppWindow->OnMouseDown.Broadcast(MouseEvent);
		}
		else if (Action == GLFW_RELEASE)
		{
			AppWindow->OnMouseUp.Broadcast(MouseEvent);
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