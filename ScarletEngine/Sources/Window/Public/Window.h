#pragma once

#include "Core.h"

namespace ScarletEngine
{
	// -----------------------------------------------------------------------------------------------------------------

	using OnWindowCloseEvent = Event<>;
	using OnWindowResizeEvent = Event<glm::ivec2>;

	// -----------------------------------------------------------------------------------------------------------------

	class ApplicationWindow
	{
	public:
		ApplicationWindow(uint32_t InWidth, uint32_t InHeight, const String& WindowTitle);

		void* GetWindowHandle() const { return WindowHandle; }
		void SwapBuffer();
		void PollEvents();

		OnWindowCloseEvent OnWindowClose;
		OnWindowResizeEvent OnWindowResize;

		uint32_t GetWidth() const { return Width; }
		uint32_t GetHeight() const { return Height; }
	private:
		void* WindowHandle;
		uint32_t Width;
		uint32_t Height;
		String WindowTitle;
	};
}