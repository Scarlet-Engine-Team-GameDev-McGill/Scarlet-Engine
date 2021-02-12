#pragma once

#include "Core.h"

namespace ScarletEngine
{
	class Window
	{
	public:
		Window(uint32_t InWidth, uint32_t InHeight, const String& WindowTitle);

		void* GetWindowHandle() const { return WindowHandle; }
		void SwapBuffer();
		void PollEvents();

		Event<>& OnWindowCloseEvent() { return OnWindowClose; }
		Event<uint32_t, uint32_t>& OnWindowResizeEvent() { return OnWindowResize; }

		uint32_t GetWidth() const { return Width; }
		uint32_t GetHeight() const { return Height; }
	private:
		void* WindowHandle = nullptr;
		uint32_t Width;
		uint32_t Height;
		String WindowTitle;

		Event<> OnWindowClose;
		Event<uint32_t, uint32_t> OnWindowResize;
	};
}