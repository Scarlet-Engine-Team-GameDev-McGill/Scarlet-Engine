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
		std::unique_ptr<Viewport> MainViewport;
		bool bViewportIsFocused;
		bool bViewportIsHovered;
		glm::vec2 ViewportSize;

		uint32_t CurrentFrameTimeIndex = 0;
		float FrameTimes[100];
	};
}