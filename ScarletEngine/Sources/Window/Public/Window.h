#pragma once

#include "Core.h"

namespace ScarletEngine
{
	class Window
	{
	public:
		Window(uint32_t Width, uint32_t Height, const String& WindowTitle);

		void* GetWindowHandle() const { return WindowHandle; }
		void SwapBuffer();
		void PollEvents();
	private:
		void* WindowHandle = nullptr;
		uint32_t Width;
		uint32_t Height;
	};
}