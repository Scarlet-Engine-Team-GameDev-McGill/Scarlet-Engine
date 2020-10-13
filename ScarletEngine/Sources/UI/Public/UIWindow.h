#pragma once

#include "UIWidget.h"

namespace ScarletEngine
{
	class UIWindow : public UIWidget
	{
	public:
		UIWindow(const String& Title, uint32_t Flags = 0);

		/** Creates a new ImGui frame for this widget and calls draw */
		virtual void Paint() final override;

		/** Called before ImGui::Begin(). Usually used to push style flags. */
		virtual void PushWindowFlags() {};
		/** Called after ImGui::End(). Usually used to pop style flags. */
		virtual void PopWindowFlags() {};
		/** Draws inside of the widget window, override-able by children */
		virtual void DrawWindowContent() = 0;

		/** Returns the title of this window */
		const String& GetWindowTitle() const { return WindowTitle; }
	protected:

		// #todo: rename
		virtual void PreDraw() {}
		virtual void PostDraw() {}

		void SetWindowTitle(const String& NewTitle);
	private:
		String WindowTitle;
		uint32_t WindowFlags;

		bool bOpen;
	};
}