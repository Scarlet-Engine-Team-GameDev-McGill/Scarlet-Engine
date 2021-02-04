#pragma once

#include "Core.h"
#include "RAL.h"

namespace ScarletEngine
{
	class SceneProxy;
	class Viewport;

	class RenderModule : public IModule
	{
	public:
		// Begin IModule interface
		IMPLEMENT_MODULE(RenderModule);

		virtual void Startup() override;
		virtual void Shutdown() override;
		virtual void PostUpdate() override;
		// End IModule interface

		Viewport* CreateViewport(uint32_t Width, uint32_t Height);
		void DrawScene(SceneProxy* Scene, Viewport* ActiveViewport);
	};
}
