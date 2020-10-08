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
		Widget->Initialize();
		Widgets.push_back(Widget);
	}

	void UILayer::RemoveWidget(const SharedPtr<UIWidget>& Widget)
	{
		auto It = std::remove(Widgets.begin(), Widgets.end(), Widget);
		if (It != Widgets.end())
		{
			(*It)->Destroy();
			Widgets.erase(It, Widgets.end());
		}
	}

	void UILayer::DrawWidgets()
	{
		for (const auto& Widget : Widgets)
		{
			Widget->Paint();
		}
	}
}