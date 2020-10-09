#include "UI/UILayer.h"

namespace ScarletEngine
{
	UILayer::~UILayer()
	{
		for (const auto& Widget : Widgets)
		{
			Widget->Destroy();
		}
		Widgets.clear();
	}

	void UILayer::AddWidget(const SharedPtr<UIWidget>& Widget)
	{
		Widget->Construct();
		Widget->SetOwningLayer(this);
		Widgets.push_back(Widget);
	}

	void UILayer::RemoveWidget(const UIWidget* WidgetToRemove)
	{
		check(WidgetToRemove != nullptr);
		WidgetsToRemove.push_back(WidgetToRemove);
	}

	void UILayer::DrawWidgets()
	{
		for (const auto& Widget : Widgets)
		{
			Widget->Paint();
		}

		RemoveQueuedWidgets();
	}

	void UILayer::RemoveQueuedWidgets()
	{
		for (const auto& WidgetToRemove : WidgetsToRemove)
		{
			auto It = std::find_if(Widgets.begin(), Widgets.end(), [WidgetToRemove](const SharedPtr<UIWidget>& Widget)
				{
					return Widget.get() == WidgetToRemove;
				});

			if (It != Widgets.end())
			{
				(*It)->Destroy();
				Widgets.erase(It);
			}
		}

		WidgetsToRemove.clear();
	}
}