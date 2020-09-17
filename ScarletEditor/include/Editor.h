#pragma once
#include "Core/Core.h"
#include "Renderer/Viewport.h"
#include <memory>
#include <glm/glm.hpp>

namespace ScarletEngine
{
	class Editor : public ITickable
	{
	public:
		Editor();

		virtual void Tick(double DeltaTime) override;
	private:
		void DrawUI();
	private:
		struct EditorViewport
		{
			EditorViewport(Viewport* InView)
				: View(InView)
				, bViewportIsFocused(false)
				, bViewportIsHovered(false)
				, ViewportSize({ 0, 0 })
			{}

			std::unique_ptr<Viewport> View;
			bool bViewportIsFocused;
			bool bViewportIsHovered;
			glm::vec2 ViewportSize;
		};
		std::vector<EditorViewport> Viewports;

		uint32_t CurrentFrameTimeIndex = 0;
		float FrameTimes[100];
	};
}