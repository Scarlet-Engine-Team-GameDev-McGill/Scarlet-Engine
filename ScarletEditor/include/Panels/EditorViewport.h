#pragma once

#include "Core/Core.h"
#include "UI/UIWidget.h"
#include "World.h"
#include "Renderer/Viewport.h"

namespace ScarletEngine
{
	class EditorViewportPanel : public UIWidget , public ITickable
	{
	public:
		EditorViewportPanel(const SharedPtr<World>& InWorld);
		virtual void Construct() override;
		virtual void Tick(double DeltaTime) override;
		virtual void Draw() override;

		virtual void PreDraw() override;
		virtual void PostDraw() override;
	private:
		static uint32_t NextViewportID;

		UniquePtr<Viewport> View;
		SharedPtr<Camera> ViewportCam;
		SharedPtr<World> RepresentingWorld;

		String ViewportTitle;

		glm::vec2 PanelSize;
		bool bViewportIsFocused;
		bool bViewportIsHovered;
	};
}