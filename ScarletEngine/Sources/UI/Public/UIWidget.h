#pragma once

#include "CoreMinimal.h"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <GoogleMaterialDesign.h>

namespace ScarletEngine
{
	class UILayer;

	class UIWidget
	{
	public:
		virtual ~UIWidget() {}
		virtual void Construct() {};
		virtual void Paint() = 0;
		virtual void Destroy() {};

		void SetOwningLayer(UILayer* Layer) { OwningLayer = Layer; }
		UILayer* GetOwningLayer() const { return OwningLayer; }
	private:
		UILayer* OwningLayer = nullptr;
	};
}