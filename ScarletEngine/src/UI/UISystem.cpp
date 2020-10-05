#include "UI/UISystem.h"

namespace ScarletEngine
{
	void UISystem::Initialize()
	{
		ActiveLayer = nullptr;
	}

	void UISystem::Shutdown()
	{
		if (ActiveLayer != nullptr)
		{
			ActiveLayer->Terminate();
			ActiveLayer.reset();
		}
	}

	void UISystem::PreUpdate() const
	{
		if (ActiveLayer != nullptr)
		{
			ActiveLayer->BeginFrame();
		}
	}

	void UISystem::PostUpdate() const
	{
		if (ActiveLayer != nullptr)
		{
			ActiveLayer->DrawWidgets();
			ActiveLayer->Draw();
		}
	}

	void UISystem::SetActiveLayer(const SharedPtr<UILayer>& InLayer)
	{
		ActiveLayer = InLayer;
		ActiveLayer->Initialize();
	}
}