#pragma once

#include "Core.h"
#include "UILayer.h"

namespace ScarletEngine
{
	class UIModule : public IModule
	{
	public:
		// Begin IModule interface
		IMPLEMENT_MODULE(UIModule, RenderModule);

		virtual void Startup() override;
		virtual void Shutdown() override;
		virtual void PreUpdate() override;
		virtual void PostUpdate() override;
		// End IModule interface

		void SetActiveLayer(const SharedPtr<UILayer>& InLayer);
	private:
		SharedPtr<UILayer> ActiveLayer;
	};
}