#include "Window.h"

#include "Core.h"
#include "AssetManager.h"
#include "GLFW/glfw3.h"
#include "InputManager.h"

namespace ScarletEngine
{
	static EKeyCode GLFWKeyToEKeyCode(int Key)
	{
		switch (Key)
		{
		case GLFW_KEY_UNKNOWN:
			return EKeyCode::KeyUnknown;
		case GLFW_KEY_SPACE:
			return EKeyCode::KeySpace;
		case GLFW_KEY_APOSTROPHE:
		 	return EKeyCode::KeyApostrophe;
		case GLFW_KEY_COMMA:
		 	return EKeyCode::KeyComma;
		case GLFW_KEY_MINUS:
		 	return EKeyCode::KeyMinus;
		case GLFW_KEY_PERIOD:
		 	return EKeyCode::KeyPeriod;
		case GLFW_KEY_SLASH:
		 	return EKeyCode::KeySlash;
		case GLFW_KEY_0:
			return EKeyCode::Key0;
		case GLFW_KEY_1:
			return EKeyCode::Key1;
		case GLFW_KEY_2:
			return EKeyCode::Key2;
		case GLFW_KEY_3:
			return EKeyCode::Key3;
		case GLFW_KEY_4:
			return EKeyCode::Key4;
		case GLFW_KEY_5:
			return EKeyCode::Key5;
		case GLFW_KEY_6:
			return EKeyCode::Key6;
		case GLFW_KEY_7:
			return EKeyCode::Key7;
		case GLFW_KEY_8:
			return EKeyCode::Key8;
		case GLFW_KEY_9:
			return EKeyCode::Key9;
		case GLFW_KEY_SEMICOLON:
		 	return EKeyCode::KeySemicolon;
		case GLFW_KEY_EQUAL:
		 	return EKeyCode::KeyEqual;
		case GLFW_KEY_A:
			return EKeyCode::KeyA;
		case GLFW_KEY_B:
			return EKeyCode::KeyB;
		case GLFW_KEY_C:
			return EKeyCode::KeyC;
		case GLFW_KEY_D:
			return EKeyCode::KeyD;
		case GLFW_KEY_E:
			return EKeyCode::KeyE;
		case GLFW_KEY_F:
			return EKeyCode::KeyF;
		case GLFW_KEY_G:
			return EKeyCode::KeyG;
		case GLFW_KEY_H:
			return EKeyCode::KeyH;
		case GLFW_KEY_I:
			return EKeyCode::KeyI;
		case GLFW_KEY_J:
			return EKeyCode::KeyJ;
		case GLFW_KEY_K:
			return EKeyCode::KeyK;
		case GLFW_KEY_L:
			return EKeyCode::KeyL;
		case GLFW_KEY_M:
			return EKeyCode::KeyM;
		case GLFW_KEY_N:
			return EKeyCode::KeyN;
		case GLFW_KEY_O:
			return EKeyCode::KeyO;
		case GLFW_KEY_P:
			return EKeyCode::KeyP;
		case GLFW_KEY_Q:
			return EKeyCode::KeyQ;
		case GLFW_KEY_R:
			return EKeyCode::KeyR;
		case GLFW_KEY_S:
			return EKeyCode::KeyS;
		case GLFW_KEY_T:
			return EKeyCode::KeyT;
		case GLFW_KEY_U:
			return EKeyCode::KeyU;
		case GLFW_KEY_V:
			return EKeyCode::KeyV;
		case GLFW_KEY_W:
			return EKeyCode::KeyW;
		case GLFW_KEY_X:
			return EKeyCode::KeyX;
		case GLFW_KEY_Y:
			return EKeyCode::KeyY;
		case GLFW_KEY_Z:
			return EKeyCode::KeyZ;
		case GLFW_KEY_LEFT_BRACKET:
		 	return EKeyCode::KeyLeftBracket;
		case GLFW_KEY_BACKSLASH:
		 	return EKeyCode::KeyBackslash;
		case GLFW_KEY_RIGHT_BRACKET:
		 	return EKeyCode::KeyRightBracket;
		case GLFW_KEY_GRAVE_ACCENT:
		 	return EKeyCode::KeyGraveAccent;
		case GLFW_KEY_WORLD_1:
		 	return EKeyCode::KeyWorld1;
		case GLFW_KEY_WORLD_2:
		 	return EKeyCode::KeyWorld2;
		case GLFW_KEY_ESCAPE:
			return EKeyCode::KeyEscape;
		case GLFW_KEY_ENTER:
			return EKeyCode::KeyEnter;
		case GLFW_KEY_TAB:
			return EKeyCode::KeyTab;
		case GLFW_KEY_BACKSPACE:
			return EKeyCode::KeyBackspace;
		case GLFW_KEY_INSERT:
			return EKeyCode::KeyInsert;
		case GLFW_KEY_DELETE:
			return EKeyCode::KeyDelete;
		case GLFW_KEY_RIGHT:
			return EKeyCode::KeyRight;
		case GLFW_KEY_LEFT:
			return EKeyCode::KeyLeft;
		case GLFW_KEY_DOWN:
			return EKeyCode::KeyDown;
		case GLFW_KEY_UP:
			return EKeyCode::KeyUp;
		case GLFW_KEY_PAGE_UP:
			return EKeyCode::KeyPageUp;
		case GLFW_KEY_PAGE_DOWN:
			return EKeyCode::KeyPageDown;
		case GLFW_KEY_HOME:
			return EKeyCode::KeyHome;
		case GLFW_KEY_END:
			return EKeyCode::KeyEnd;
		case GLFW_KEY_CAPS_LOCK:
			return EKeyCode::KeyCapsLock;
		case GLFW_KEY_SCROLL_LOCK:
			return EKeyCode::KeyScrollLock;
		case GLFW_KEY_NUM_LOCK:
			return EKeyCode::KeyNumLock;
		case GLFW_KEY_PRINT_SCREEN:
			return EKeyCode::KeyPrintScreen;
		case GLFW_KEY_PAUSE:
			return EKeyCode::KeyPause;
		case GLFW_KEY_F1:
			return EKeyCode::KeyF1;
		case GLFW_KEY_F2:
			return EKeyCode::KeyF2;
		case GLFW_KEY_F3:
			return EKeyCode::KeyF3;
		case GLFW_KEY_F4:
			return EKeyCode::KeyF4;
		case GLFW_KEY_F5:
			return EKeyCode::KeyF5;
		case GLFW_KEY_F6:
			return EKeyCode::KeyF6;
		case GLFW_KEY_F7:
			return EKeyCode::KeyF7;
		case GLFW_KEY_F8:
			return EKeyCode::KeyF8;
		case GLFW_KEY_F9:
			return EKeyCode::KeyF9;
		case GLFW_KEY_F10:
			return EKeyCode::KeyF10;
		case GLFW_KEY_F11:
			return EKeyCode::KeyF11;
		case GLFW_KEY_F12:
			return EKeyCode::KeyF12;
		case GLFW_KEY_F13:
			return EKeyCode::KeyF13;
		case GLFW_KEY_F14:
			return EKeyCode::KeyF14;
		case GLFW_KEY_F15:
			return EKeyCode::KeyF15;
		case GLFW_KEY_F16:
			return EKeyCode::KeyF16;
		case GLFW_KEY_F17:
			return EKeyCode::KeyF17;
		case GLFW_KEY_F18:
			return EKeyCode::KeyF18;
		case GLFW_KEY_F19:
			return EKeyCode::KeyF19;
		case GLFW_KEY_F20:
			return EKeyCode::KeyF20;
		case GLFW_KEY_F21:
			return EKeyCode::KeyF21;
		case GLFW_KEY_F22:
			return EKeyCode::KeyF22;
		case GLFW_KEY_F23:
			return EKeyCode::KeyF23;
		case GLFW_KEY_F24:
			return EKeyCode::KeyF24;
		case GLFW_KEY_F25:
			return EKeyCode::KeyF25;
		case GLFW_KEY_KP_0:
			return EKeyCode::KeyNum0;
		case GLFW_KEY_KP_1:
			return EKeyCode::KeyNum1;
		case GLFW_KEY_KP_2:
			return EKeyCode::KeyNum2;
		case GLFW_KEY_KP_3:
			return EKeyCode::KeyNum3;
		case GLFW_KEY_KP_4:
			return EKeyCode::KeyNum4;
		case GLFW_KEY_KP_5:
			return EKeyCode::KeyNum5;
		case GLFW_KEY_KP_6:
			return EKeyCode::KeyNum6;
		case GLFW_KEY_KP_7:
			return EKeyCode::KeyNum7;
		case GLFW_KEY_KP_8:
			return EKeyCode::KeyNum8;
		case GLFW_KEY_KP_9:
			return EKeyCode::KeyNum9;
		case GLFW_KEY_KP_DECIMAL:
			return EKeyCode::KeyNumDecimal;
		case GLFW_KEY_KP_DIVIDE:
			return EKeyCode::KeyNumDivide;
		case GLFW_KEY_KP_MULTIPLY:
			return EKeyCode::KeyNumMultiply;
		case GLFW_KEY_KP_SUBTRACT:
			return EKeyCode::KeyNumSubtract;
		case GLFW_KEY_KP_ADD:
			return EKeyCode::KeyNumAdd;
		case GLFW_KEY_KP_ENTER:
			return EKeyCode::KeyNumEnter;
		case GLFW_KEY_KP_EQUAL:
			return EKeyCode::KeyNumEqual;
		case GLFW_KEY_LEFT_SHIFT:
			return EKeyCode::KeyLeftShift;
		case GLFW_KEY_LEFT_CONTROL:
			return EKeyCode::KeyLeftControl;
		case GLFW_KEY_LEFT_ALT:
			return EKeyCode::KeyLeftAlt;
		case GLFW_KEY_LEFT_SUPER:
			return EKeyCode::KeyLeftSuper;
		case GLFW_KEY_RIGHT_SHIFT:
			return EKeyCode::KeyRightShift;
		case GLFW_KEY_RIGHT_CONTROL:
			return EKeyCode::KeyRightControl;
		case GLFW_KEY_RIGHT_ALT:
			return EKeyCode::KeyRightAlt;
		case GLFW_KEY_RIGHT_SUPER:
			return EKeyCode::KeyRightSuper;
		case GLFW_KEY_MENU:
			return EKeyCode::KeyMenu;
		default:
			check(false);
			return EKeyCode::KeyUnknown;
		}
	}

	static EMouseCode GLFWMouseToEMouseCode(int Button)
	{
		switch (Button)
		{
		case GLFW_MOUSE_BUTTON_1:
			return EMouseCode::MouseButton1;
		case GLFW_MOUSE_BUTTON_2:
			return EMouseCode::MouseButton2;
		case GLFW_MOUSE_BUTTON_3:
			return EMouseCode::MouseButton3;
		case GLFW_MOUSE_BUTTON_4:
			return EMouseCode::MouseButton4;
		case GLFW_MOUSE_BUTTON_5:
			return EMouseCode::MouseButton5;
		case GLFW_MOUSE_BUTTON_6:
			return EMouseCode::MouseButton6;
		case GLFW_MOUSE_BUTTON_7:
			return EMouseCode::MouseButton7;
		case GLFW_MOUSE_BUTTON_8:
			return EMouseCode::MouseButton8;
		default:
			return EMouseCode::MouseUnknown;
		}
	}

	static void WindowCloseCallback(GLFWwindow* WindowHandle)
	{
		ApplicationWindow* AppWindow = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(WindowHandle));
		
		AppWindow->OnWindowClose.Broadcast();
	}

	static void WindowResizeCallback(GLFWwindow* WindowHandle, int Width, int Height)
	{
		ApplicationWindow* AppWindow = static_cast<ApplicationWindow*>(glfwGetWindowUserPointer(WindowHandle));

		AppWindow->OnWindowResize.Broadcast({ Width, Height });
	}

	static void KeyCallback(GLFWwindow*, int Key, int, int Action, int Mods)
	{
		if (Action == GLFW_PRESS)
		{
			InputManager::Get().OnKeyDownCallback(GLFWKeyToEKeyCode(Key));
		}
		else if (Action == GLFW_RELEASE)
		{
			InputManager::Get().OnKeyUpCallback(GLFWKeyToEKeyCode(Key));
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
			InputManager::Get().OnMousePressCallback(GLFWMouseToEMouseCode(Button));
		}
		else if (Action == GLFW_RELEASE)
		{
			InputManager::Get().OnMouseReleaseCallback(GLFWMouseToEMouseCode(Button));
		}
	}

	ApplicationWindow::ApplicationWindow(uint32_t InWidth, uint32_t InHeight, const String& InWindowTitle)
		: WindowHandle(nullptr)
		, Width(InWidth)
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
		glfwMaximizeWindow(static_cast<GLFWwindow*>(WindowHandle));

		glfwSetWindowUserPointer(static_cast<GLFWwindow*>(WindowHandle), this);

		// Set the window icon;
		const SharedPtr<TextureHandle> LogoTex = AssetManager::LoadTextureFile("/ScarletEngine/Content/scarlet_logo.png");
		GLFWimage Image;
		Image.pixels = LogoTex->PixelDataBuffer;
		Image.width = LogoTex->Width;
		Image.height = LogoTex->Height;

		glfwSetWindowIcon(static_cast<GLFWwindow*>(WindowHandle), 1, &Image);

		glfwSetWindowCloseCallback(static_cast<GLFWwindow*>(WindowHandle), WindowCloseCallback);
		glfwSetFramebufferSizeCallback(static_cast<GLFWwindow*>(WindowHandle), WindowResizeCallback);
		glfwSetKeyCallback(static_cast<GLFWwindow*>(WindowHandle), KeyCallback);
		glfwSetCursorPosCallback(static_cast<GLFWwindow*>(WindowHandle), CursorPosCallback);
		glfwSetMouseButtonCallback(static_cast<GLFWwindow*>(WindowHandle), MouseButtonCallback);
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