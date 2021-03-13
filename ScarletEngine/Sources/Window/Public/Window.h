#pragma once

#include "Core.h"
#include "KeyInputEvent.h"
#include "MouseInputEvent.h"
#include "CursorMoveEvent.h"

namespace ScarletEngine
{
	class ApplicationWindow
	{
	public:
		ApplicationWindow(uint32_t InWidth, uint32_t InHeight, const String& WindowTitle);

		void* GetWindowHandle() const { return WindowHandle; }
		void SwapBuffer();
		void PollEvents();

		Event<> OnWindowClose;
		Event<uint32_t, uint32_t> OnWindowResize;
		Event<CursorMoveEvent> OnCursorMove;
		Event<MouseInputEvent> OnMouseDown;
		Event<MouseInputEvent> OnMouseUp;
		Event<KeyInputEvent> OnKeyDown;
		Event<KeyInputEvent> OnKeyUp;

		uint32_t GetWidth() const { return Width; }
		uint32_t GetHeight() const { return Height; }

		double GetLastCursorXPos() const { return LastCursorXPos; }
		double GetLastCursorYPos() const { return LastCursorYPos; }
	private:
		void* WindowHandle;
		uint32_t Width;
		uint32_t Height;
		String WindowTitle;

		double LastCursorXPos;
		double LastCursorYPos;
	};
}