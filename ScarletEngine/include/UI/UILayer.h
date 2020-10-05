#pragma once

#include "Core/CoreMinimal.h"
#include "UI/UIWidget.h"

namespace ScarletEngine
{
	class UILayer
	{
	public:
		virtual void Initialize() = 0;
		virtual void BeginFrame() = 0;
		virtual void Draw() = 0;
		virtual void Terminate();

		void AddWidget(const SharedPtr<UIWidget>& Widget);
		void RemoveWidget(const SharedPtr<UIWidget>& Widget);
		void DrawWidgets();
	private:
		Array<SharedPtr<UIWidget>> Widgets;
	};
}