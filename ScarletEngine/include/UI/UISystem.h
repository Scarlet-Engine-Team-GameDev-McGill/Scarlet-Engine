#pragma once

#include "Core/CoreMinimal.h"
#include "UI/UILayer.h"

namespace ScarletEngine
{
	class UISystem
	{
	public:
		void Initialize();
		void Shutdown();

		void PreUpdate() const;
		void PostUpdate() const;

		void SetActiveLayer(const SharedPtr<UILayer>& InLayer);

		static UISystem& Get() { static UISystem Instance; return Instance; }
	private:
		SharedPtr<UILayer> ActiveLayer;
	};
}