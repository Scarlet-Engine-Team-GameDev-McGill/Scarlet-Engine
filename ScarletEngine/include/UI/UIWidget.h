#pragma once

#include "Core/CoreMinimal.h"

namespace ScarletEngine
{
	class UIWidget
	{
	public:
		UIWidget(const String& Title, uint32_t Flags = 0);
		/** Creates a new ImGui frame for this widget and calls draw */
		void Paint();

		virtual void Construct() {};
		virtual void Destroy() {};

		// #todo: rename
		virtual void PreDraw() {}
		virtual void PostDraw() {}
		/** Draws inside of the widget window, override-able by children */
		virtual void Draw() = 0;

	protected:
		void SetWindowTitle(const String& NewTitle);
	private:
		friend class UILayer;

		String WindowTitle;
		UILayer* OwningLayer;
		uint32_t WindowFlags;

		bool bOpen;
	};
}