#pragma once

#include "Core/CoreMinimal.h"
#include "UI/UIWidget.h"

namespace ScarletEngine
{
	class UILayer
	{
	public:
		virtual ~UILayer();

		virtual void Initialize() {};
		virtual void BeginFrame() {};
		virtual void Draw() = 0;
		virtual void Terminate() {};

		void AddWidget(const SharedPtr<UIWidget>& Widget);
		void RemoveWidget(const UIWidget* WidgetToRemove);
		void DrawWidgets();
	private:
		void RemoveQueuedWidgets();
	private:
		Array<SharedPtr<UIWidget>> Widgets;
		Array<const UIWidget*> WidgetsToRemove;
	};
}