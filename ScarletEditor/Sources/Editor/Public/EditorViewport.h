#pragma once

#include "Core.h"
#include "UIWindow.h"
#include "World.h"
#include "Viewport.h"

namespace ScarletEngine
{
	class EditorViewportPanel : public UIWindow , public ITickable
	{
	public:
		EditorViewportPanel(const SharedPtr<World>& InWorld);
		virtual void Construct() override;
		virtual void Tick(double DeltaTime) override;
		virtual void DrawWindowContent() override;

		virtual void PushWindowFlags() override;
		virtual void PopWindowFlags() override;
	private:
		static uint32_t NextViewportID;

		UniquePtr<Viewport> View;
		SharedPtr<Camera> ViewportCam;
		SharedPtr<World> RepresentingWorld;

		String ViewportTitle;

		glm::vec2 PanelSize;
		bool bViewportIsFocused;
		bool bViewportIsHovered;
		bool bShowGrid;
	};
}