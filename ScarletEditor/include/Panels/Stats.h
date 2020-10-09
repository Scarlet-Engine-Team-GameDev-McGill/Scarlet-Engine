#pragma once

#include "Core/Core.h"
#include "UI/UIWindow.h"

namespace ScarletEngine
{
	class StatsPanel : public UIWindow, ITickable
	{
	public:
		StatsPanel();

		virtual void Tick(double DeltaTime) override;
		virtual void DrawWindowContent() override;
	private:
		static const int FrameWindowSize = 50;
		float FrameTimes[FrameWindowSize];

		float FrameTimeSum = 0.f;
		float FrameTimeMean = 0.f;

		uint32_t CurrentFrameTimeIndex = 0;
	};
}